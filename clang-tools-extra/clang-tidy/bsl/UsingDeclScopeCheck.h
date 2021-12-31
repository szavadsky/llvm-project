//===--- UsingDeclScopeCheck.h - clang-tidy ---------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BSL_USINGDECLSCOPECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BSL_USINGDECLSCOPECHECK_H

#include "../ClangTidyCheck.h"
#include "../utils/FileExtensionsUtils.h"

namespace clang {
namespace tidy {
namespace bsl {

/// Warns whenever a using declaration is found outside of function or
/// class scope while inside of a header file.
///
/// The check supports these options:
///   - `HeaderFileExtensions`: a semicolon-separated list of filename
///     extensions of header files (the filename extensions should not contain
///     "." prefix). ";h;hh;hpp;hxx" by default.
///
///     For extension-less header files, using an empty string or leaving an
///     empty string between ";" if there are other filename extensions.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/bsl-using-decl-scope.html
class UsingDeclScopeCheck : public ClangTidyCheck {
public:
  UsingDeclScopeCheck(StringRef Name, ClangTidyContext *Context);
  void storeOptions(ClangTidyOptions::OptionMap &Opts) override;
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

private:
  const std::string RawStringHeaderFileExtensions;
  utils::FileExtensionsSet HeaderFileExtensions;
};

} // namespace bsl
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BSL_USINGDECLSCOPECHECK_H
