#ifndef OMNIFUZZ_INSTRUMENTATION_H
#define OMNIFUZZ_INSTRUMENTATION_H

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InlineAsm.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/raw_ostream.h"

namespace llvm {

template <class FeedbackT>
class OmnifuzzInstrumentationPass : public PassInfoMixin<OmnifuzzInstrumentationPass<FeedbackT>> {
 public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
    for (auto &F: M) {
      for (auto &BB: F) {
        IRBuilder<> IRB(BB.getFirstNonPHI());
        std::string AsmStr;
        raw_string_ostream AsmPerBlock(AsmStr);
        AsmPerBlock << "nop\nnop\nnop\n";
        SmallVector<Value*, 16> AsmArgs;
        InlineAsm *BlockAsm =
          InlineAsm::get(FunctionType::get(IRB.getVoidTy(), {}, 
             false), AsmPerBlock.str(), "", false);
        IRB.CreateCall(BlockAsm, {});
      }
    }
    return PreservedAnalyses::all();
  }
 private:
  FeedbackT _feedback;
};

} // namespace llvm


#endif  // OMNIFUZZ_INSTRUMENTATION_H
