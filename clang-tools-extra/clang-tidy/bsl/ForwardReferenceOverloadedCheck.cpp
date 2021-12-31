//===--- ForwardReferenceOverloadedCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ForwardReferenceOverloadedCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"

#include <iostream>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bsl {

AST_MATCHER(FunctionDecl, isCopyOrMove) {
  if (isa<CXXConstructorDecl>(&Node)) {
    return dyn_cast<CXXConstructorDecl>(&Node)->isCopyOrMoveConstructor();
  } else if (isa<CXXMethodDecl>(&Node)) {
    return dyn_cast<CXXMethodDecl>(&Node)->isCopyAssignmentOperator() ||
           dyn_cast<CXXMethodDecl>(&Node)->isMoveAssignmentOperator();
  }
  return false;
}

void ForwardReferenceOverloadedCheck::registerMatchers(MatchFinder *Finder) {
 auto ForwardingReferenceParmMatcher =
  parmVarDecl(
    hasType(
      qualType(
        rValueReferenceType(),
        references(
          templateTypeParmType(
            hasDeclaration(
              templateTypeParmDecl().bind("type-parm-decl")
            )
          )
        ),
        unless(
          references(
            qualType(
              isConstQualified()
            )
          )
        )
      )
    )
  ).bind("parm-var");

  Finder->addMatcher(
    ForwardingReferenceParmMatcher,
    this
  );

  Finder->addMatcher(
    functionDecl(
      unless(
        anyOf(
          isDeleted(),
          isCopyOrMove(),
          isImplicit()
        )
      )
    ).bind("func-decl"),
    this
  );
}

void ForwardReferenceOverloadedCheck::check(
    const MatchFinder::MatchResult &Result)
{
  std::string name;

  if (auto const *FD = Result.Nodes.getNodeAs<FunctionDecl>("func-decl")) {
    name = FD->getQualifiedNameAsString();

    // Ignore prototypes
    if (FD->getDefinition() != FD)
      return;

    // Ignore Specializations
    if (FD->getMemberSpecializationInfo())
      return;

    // Ignore Specializations
    if (FD->getTemplateSpecializationInfo())
      return;

    m_fds[name].push_back(FD);
  }
  else {
    auto const *PVD = Result.Nodes.getNodeAs<ParmVarDecl>("parm-var");
    auto const *TTPD = Result.Nodes.getNodeAs<TemplateTypeParmDecl>("type-parm-decl");

    // If this is actually a forward reference, add it to the list of
    // parameters that are for references.

    auto const *parentDC = PVD->getParentFunctionOrMethod();
    if (parentDC == nullptr)
      return;

    auto const *parentFD = dyn_cast<FunctionDecl>(parentDC);
    if (parentFD == nullptr)
      return;

    name = parentFD->getQualifiedNameAsString();

    auto const *parentFTD = parentFD->getDescribedFunctionTemplate();
    if (parentFTD == nullptr)
      return;

    auto const *templateParameters = parentFTD->getTemplateParameters();
    if (templateParameters == nullptr)
      return;

    if (!llvm::is_contained(*templateParameters, TTPD))
      return;

    m_fr_params.insert(PVD);
  }

  auto const &fdOverloads{m_fds[name]};

  if (fdOverloads.size() <= 1)
    return;

  for (auto const *fd1 : fdOverloads) {
    for (auto const *fd2 : fdOverloads) {
      if (fd1 == fd2)
        continue;

      auto const parametersList1{fd1->parameters()};
      auto const parametersList2{fd2->parameters()};

      if (parametersList1.size() != parametersList2.size())
        continue;

      ParmVarDecl const *foundParam1{};
      ParmVarDecl const *foundParam1Previous{};
      ParmVarDecl const *foundParam2{};
      ParmVarDecl const *foundParam2Previous{};

      for (size_t i{}; i < parametersList1.size(); ++i) {
        auto const *param1{parametersList1[i]};
        auto const *param2{parametersList2[i]};

        if (param1->getType().getTypePtr() != param2->getType().getTypePtr()) {
          auto iter1 = m_fr_params.find(param1);
          auto iter2 = m_fr_params.find(param2);

          if (iter1 != m_fr_params.end()) {
            foundParam1 = param1;
            foundParam1Previous = param2;
            continue;
          }

          if (iter2 != m_fr_params.end()) {
            foundParam2 = param2;
            foundParam2Previous = param1;
            continue;
          }

          foundParam1 = nullptr;
          foundParam2 = nullptr;
        }
      }

      if (foundParam1 != nullptr) {
        diag(foundParam1->getLocation(), "A function that contains an ambiguous forwarding "
                                         "reference as an argument shall not be overloaded.");
        diag(foundParam1Previous->getLocation(), "previous argument found here", DiagnosticIDs::Note);
      }

      if (foundParam2 != nullptr) {
        diag(foundParam2->getLocation(), "A function that contains an ambiguous forwarding "
                                         "reference as an argument shall not be overloaded.");
        diag(foundParam2Previous->getLocation(), "previous argument found here", DiagnosticIDs::Note);
      }
    }
  }
}

} // namespace bsl
} // namespace tidy
} // namespace clang
