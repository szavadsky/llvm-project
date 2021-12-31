//===--- ClassMemberInitCheck.h - clang-tidy --------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BSL_CLASSMEMBERINITCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BSL_CLASSMEMBERINITCHECK_H

#include "../ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace bsl {

/// Warns in any of the following cases:
///   - class has fields that use a mixture of in-class initializers
///     and constructor initializers
///   - constructor doesn't init each field with member initialization list
///     (unless they are initialized in-class)
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/bsl-class-member-init.html
class ClassMemberInitCheck : public ClangTidyCheck {
public:
  ClassMemberInitCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus11;
  }

private:
  void checkCtorWithInit(const CXXConstructorDecl *Ctor);
  void checkCtorWithoutInit(const CXXConstructorDecl *Ctor);
};

} // namespace bsl
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BSL_CLASSMEMBERINITCHECK_H
