#ifndef OMNIFUZZ_PASS_H
#define OMNIFUZZ_PASS_H

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/PassManager.h"

namespace llvm {
  class OmnifuzzInstrumentationPass : public PassInfoMixin<OmnifuzzInstrumentationPass> {
   public:
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
    static bool isRequired() {return true;}
   private:
    bool initialization(Module*);
    void instrumentOn(BasicBlock&);
  };
} // namespace llvm

#endif // OMNIFUZZ_PASS_H
