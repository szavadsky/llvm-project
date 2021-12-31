//===--- EnumInitCheck.cpp - clang-tidy ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "EnumInitCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bsl {

void EnumInitCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(enumDecl().bind("enum"), this);
}

void EnumInitCheck::check(const MatchFinder::MatchResult &Result) {
  auto Enum = Result.Nodes.getNodeAs<EnumDecl>("enum");

  auto Loc = Enum->getLocation();
  if (Loc.isInvalid() || Loc.isMacroID())
    return;

  std::vector<std::pair<SourceLocation, Expr *>> InitExprs;

  // Save each EnumConstantDecl's init expression along with
  // its source location.
  for (auto Iter = Enum->enumerator_begin();
       Iter != Enum->enumerator_end();
       ++Iter) {
    auto EnumConstant = *Iter;
    auto Loc = EnumConstant->getSourceRange().getBegin();
    auto Exp = EnumConstant->getInitExpr();

    InitExprs.push_back({Loc, Exp});
  }

  // Iterate over the entries, stopping to diagnose if we detect a
  // transition from nullptr -> !nullptr or !nullptr -> nullptr
  // of the init Expr*'s.
  for (auto i = 1U; i < InitExprs.size(); ++i) {
    if ((InitExprs[i].second && !InitExprs[i - 1U].second) ||
        ((i > 1U) && (!InitExprs[i].second && InitExprs[i - 1U].second))) {
        diagNullExprs(InitExprs);
        return;
    }
  }

}

void EnumInitCheck::diagNullExprs(
  const std::vector<std::pair<SourceLocation, Expr *>> &InitExprs) {
  for (auto i = 0U; i < InitExprs.size(); ++i) {
    if (InitExprs[i].second)
      continue;
    diag(InitExprs[i].first, "uninitialized enum constant");
  }
}

} // namespace bsl
} // namespace tidy
} // namespace clang
