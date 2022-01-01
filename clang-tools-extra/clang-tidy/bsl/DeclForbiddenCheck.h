//===--- DeclForbiddenCheck.h - clang-tidy ----------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BSL_DECLFORBIDDENCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BSL_DECLFORBIDDENCHECK_H

#include "../ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace bsl {

/// Warn if any of the following declarations are found:
/// - unions
/// - bitfields
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/bsl-decl-forbidden.html
class DeclForbiddenCheck : public ClangTidyCheck {
public:
  DeclForbiddenCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus11;
  }
};

} // namespace bsl
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BSL_DECLFORBIDDENCHECK_H
