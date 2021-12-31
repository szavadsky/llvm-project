//===--- LambdaParamListCheck.cpp - clang-tidy ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "LambdaParamListCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bsl {

void LambdaParamListCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(lambdaExpr().bind("lambda"), this);
}

void LambdaParamListCheck::check(const MatchFinder::MatchResult &Result) {
  auto Lambda = Result.Nodes.getNodeAs<LambdaExpr>("lambda");
  auto Loc = Lambda->getBeginLoc();

  if (Loc.isInvalid() || Loc.isMacroID())
    return;

  if (Lambda->hasExplicitParameters())
    return;

  diag(Loc, "lambda expression with implicit parameter list");
}

} // namespace bsl
} // namespace tidy
} // namespace clang
