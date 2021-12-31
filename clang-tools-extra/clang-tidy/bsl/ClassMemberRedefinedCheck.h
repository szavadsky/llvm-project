//===--- ClassMemberRedefinedCheck.h - clang-tidy ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BSL_CLASSMEMBERREDEFINEDCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BSL_CLASSMEMBERREDEFINEDCHECK_H

#include "../ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace bsl {

/// Warns if a derived class defines a data member or method that hides a
/// member inherited from one of its base classes.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/bsl-class-member-redefined.html
class ClassMemberRedefinedCheck : public ClangTidyCheck {
public:
  ClassMemberRedefinedCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus11;
  }

private:
  bool FoundHidden{false};
  SourceLocation HiddenMemberLoc{};
  StringRef HiddenMemberClassName{};

  bool hasNoMemberWithName(const CXXRecordDecl *Base, StringRef MemberName);
};

} // namespace bsl
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BSL_CLASSMEMBERREDEFINEDCHECK_H
