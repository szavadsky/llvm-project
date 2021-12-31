//===--- ForwardReferenceOverloadedCheck.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BSL_FORWARDREFERENCEOVERLOADEDCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BSL_FORWARDREFERENCEOVERLOADEDCHECK_H

#include "../ClangTidyCheck.h"
#include <set>
#include <unordered_map>

namespace clang {
namespace tidy {
namespace bsl {

class ForwardReferenceOverloadedCheck : public ClangTidyCheck {
public:
  ForwardReferenceOverloadedCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus11;
  }

private:
  std::set<ParmVarDecl const *> m_fr_params;
  std::unordered_map<std::string, std::list<FunctionDecl const *>> m_fds;
};

} // namespace bsl
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BSL_FORWARDREFERENCEOVERLOADEDCHECK_H
