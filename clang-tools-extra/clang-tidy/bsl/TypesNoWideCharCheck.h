//===--- TypesNoWideCharCheck.h - clang-tidy --------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BSL_TYPESNOWIDECHARCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BSL_TYPESNOWIDECHARCHECK_H

#include "../ClangTidyCheck.h"
#include <memory>

namespace clang {

class IdentifierTable;

namespace tidy {
namespace bsl {

/// Checks that no wide-character types are used
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/bsl-types-no-wide-char.html
class TypesNoWideCharCheck : public ClangTidyCheck {
public:
  TypesNoWideCharCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  std::unique_ptr<IdentifierTable> IdentTable;
};

} // namespace bsl
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BSL_TYPESNOWIDECHARCHECK_H
