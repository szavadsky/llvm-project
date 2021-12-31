//===--- TypesNoWideCharCheck.cpp - clang-tidy ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TypesNoWideCharCheck.h"
#include "BslCheckUtils.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bsl {

void TypesNoWideCharCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(typeLoc(loc(isInteger())).bind("loc"), this);
  IdentTable = std::make_unique<IdentifierTable>(getLangOpts());
}

void TypesNoWideCharCheck::check(const MatchFinder::MatchResult &Result) {
  auto TL = *Result.Nodes.getNodeAs<TypeLoc>("loc");
  auto Loc = TL.getBeginLoc();

  if (Loc.isInvalid() || Loc.isMacroID())
    return;

  if (auto QL = TL.getAs<QualifiedTypeLoc>())
    TL = QL.getUnqualifiedLoc();

  auto Builtin = TL.getAs<BuiltinTypeLoc>();
  if (!Builtin)
    return;

  auto Tok = getTokenAtLoc(Loc, Result, *IdentTable);
  if (!Tok.isOneOf(tok::kw_char16_t, tok::kw_char32_t, tok::kw_wchar_t))
    return;

  diag(Loc, "%0 is wide character type") << Builtin.getType();
}

} // namespace bsl
} // namespace tidy
} // namespace clang
