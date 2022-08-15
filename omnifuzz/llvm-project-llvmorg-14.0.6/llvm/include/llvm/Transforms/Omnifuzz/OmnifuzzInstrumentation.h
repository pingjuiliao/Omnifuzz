#ifndef OMNIFUZZ_INSTRUMENTATION_H
#define OMNIFUZZ_INSTRUMENTATION_H

#include "llvm/IR/PassManager.h"

namespace llvm {

template <class FeedbackT>
class OmnifuzzInstrumentationPass : public PassInfoMixin<OmnifuzzInstrumentationPass<FeedbackT>> {
 public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    for (auto &BB: F) {
      errs() << "Basicblock size: " <<  BB.size() << "\n";
      _feedback.operatesOnBasicBlock(BB.size());
    }
    return PreservedAnalyses::all();
  }
 private:
  FeedbackT _feedback;
};

} // namespace llvm


#endif  // OMNIFUZZ_INSTRUMENTATION_H
