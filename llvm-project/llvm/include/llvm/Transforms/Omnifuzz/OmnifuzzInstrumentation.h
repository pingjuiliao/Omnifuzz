#ifndef OMNIFUZZ_INSTRUMENTATION_H
#define OMNIFUZZ_INSTRUMENTATION_H

#include <sstream>

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InlineAsm.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/raw_ostream.h"
#include "omnifuzz/feedback/feedback_mechanism.h"

namespace llvm {

class OmnifuzzPass : public PassInfoMixin<OmnifuzzPass> {
 public:
  OmnifuzzPass(std::unique_ptr<omnifuzz::FeedbackMechanism>);
  PreservedAnalyses run(Module&, ModuleAnalysisManager&);
  
 private:
  void createForkserverFunction(Module&);
  bool initialize(Module&);
  void instrumentBasicBlockAssembly(BasicBlock& BB);
  void instrumentEmbeddedForkserver(BasicBlock* BB,
                                    BasicBlock* SuccessBB, 
                                    BasicBlock* RetBB);
  std::unique_ptr<omnifuzz::FeedbackMechanism> fdbk_mech_;

  GlobalVariable* InitGV;
  GlobalVariable* FailGV; 
  GlobalVariable* ShmPtrGV;
  Function* ForkserverInitFunction;
  // Value* InitForksrvFunc;
};

} // namespace llvm


#endif  // OMNIFUZZ_INSTRUMENTATION_H
