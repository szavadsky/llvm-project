//===--- LiteralsAsciiOnlyCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "LiteralsAsciiOnlyCheck.h"
#include "BslCheckUtils.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bsl {

void LiteralsAsciiOnlyCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(stringLiteral().bind("str-lit"), this);
}

void LiteralsAsciiOnlyCheck::check(const MatchFinder::MatchResult &Result) {
  auto Lit = Result.Nodes.getNodeAs<StringLiteral>("str-lit");
  if (!Lit)
    return;

  auto Loc = Lit->getBeginLoc();
  if (Loc.isInvalid() || Loc.isMacroID())
    return;

  if (Lit->isAscii())
    return;

  diag(Loc, "non-ascii string literal");
}

} // namespace bsl
} // namespace tidy
} // namespace clang
