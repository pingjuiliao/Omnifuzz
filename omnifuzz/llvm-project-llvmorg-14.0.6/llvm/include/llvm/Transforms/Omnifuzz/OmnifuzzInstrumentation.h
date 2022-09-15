#ifndef OMNIFUZZ_INSTRUMENTATION_H
#define OMNIFUZZ_INSTRUMENTATION_H

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InlineAsm.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Omnifuzz/Feedback/feedback.h"

namespace llvm {

class OmnifuzzPass : public PassInfoMixin<OmnifuzzPass> {
 public:
  OmnifuzzPass(std::unique_ptr<omnifuzz::Feedback>);
  PreservedAnalyses run(Module&, ModuleAnalysisManager&);
  
 private:
  bool initialize(Module&);
  void instrumentsBasicBlockAssembly(BasicBlock& BB);
  std::unique_ptr<omnifuzz::Feedback> feedback_;
};

} // namespace llvm


#endif  // OMNIFUZZ_INSTRUMENTATION_H
