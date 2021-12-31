//===--- OpBitwiseOperandsCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "OpBitwiseOperandsCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Tooling/FixIt.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bsl {

void OpBitwiseOperandsCheck::registerMatchers(MatchFinder *Finder) {
  auto BinaryOps = hasAnyOperatorName("^", "|", "&", "^=", "|=", "&=",
                                      "<<", ">>", "<<=", ">>=");
  Finder->addMatcher(binaryOperator(BinaryOps,
                                    unless(isTypeDependent())).bind("op"),
                     this);
}

void OpBitwiseOperandsCheck::check(const MatchFinder::MatchResult &Result) {
  auto Op = Result.Nodes.getNodeAs<BinaryOperator>("op");
  auto Loc = Op->getOperatorLoc();

  if (Loc.isInvalid() || Loc.isMacroID())
    return;

  auto LHS = Op->getLHS()->IgnoreImpCasts();
  auto RHS = Op->getRHS()->IgnoreImpCasts();

  QualType LHSType = LHS->getType().getCanonicalType();
  QualType RHSType = RHS->getType().getCanonicalType();

  if (LHSType.getTypePtr() == RHSType.getTypePtr()) {
    return;
  }

  auto LText = tooling::fixit::getText(
      LHS->getSourceRange(), *Result.Context);
  auto RText = tooling::fixit::getText(
      RHS->getSourceRange(), *Result.Context);

  diag(Loc, "operands of bitwise operator have unequal types"
            " (%0 has type '%1', %2 has type '%3')") <<
      LText << LHSType.getAsString() <<
      RText << RHSType.getAsString();
}

} // namespace bsl
} // namespace tidy
} // namespace clang
