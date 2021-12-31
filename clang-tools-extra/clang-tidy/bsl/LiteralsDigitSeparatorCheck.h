//===--- LiteralsDigitSeparatorCheck.h - clang-tidy -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BSL_LITERALSDIGITSEPARATORCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BSL_LITERALSDIGITSEPARATORCHECK_H

#include "../ClangTidyCheck.h"
#include <memory>

namespace clang {
namespace tidy {
namespace bsl {

/// Warns if any of the following do not hold for digit separators in
/// integer literals:
///
/// - Hex must have one separator per two digits
/// - Binary must have one separator per four digits
/// - Decimal must have one separator per three digits
///
/// Literals without a digit separator are ignored
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/bsl-literals-digit-separator.html
class LiteralsDigitSeparatorCheck : public ClangTidyCheck {
public:
  LiteralsDigitSeparatorCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus14;
  }

private:
  std::unique_ptr<llvm::Regex> Hex{};
  std::unique_ptr<llvm::Regex> Bin{};
  std::unique_ptr<llvm::Regex> Dec{};
};

} // namespace bsl
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BSL_LITERALSDIGITSEPARATORCHECK_H
