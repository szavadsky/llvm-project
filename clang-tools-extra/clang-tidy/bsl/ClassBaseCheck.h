//===--- ClassBaseCheck.h - clang-tidy --------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BSL_CLASSBASECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BSL_CLASSBASECHECK_H

#include "../ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace bsl {

/// Warns if a class has more than one base class that is not an interface
/// class. This check considers a class to be an "interface class" if
///    1) it has a virtual destructor
///    2) it has at least one public pure virtual method
///    3) its non-special methods are public pure virtual
///    4) it has no dependent bases
///    5) its base classes (if any) are interfaces
///    6) its data members (if any) are public static constexpr
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/bsl-class-base.html
class ClassBaseCheck : public ClangTidyCheck {
public:
  ClassBaseCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus11;
  }

private:
  bool isInterfaceClass(const CXXRecordDecl *Base);
  bool isInterfaceMethod(const CXXMethodDecl *MD);
  bool isInterfaceVar(const VarDecl *VD);

  unsigned int InterfaceBases{};
};

} // namespace bsl
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BSL_CLASSBASECHECK_H
