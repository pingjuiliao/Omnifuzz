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

  // LLVM Module pass.
  PreservedAnalyses run(Module&, ModuleAnalysisManager&);
  
 private:
  
  // Create Data Member ForkserverInitFunction, which serves as  
  // an embedded forkserver in the program under test.
  void createForkserverFunction(Module&);
  
  // Declare variables for runtime operations.
  bool performDeclaration(Module&);

  // Per-basicblock instrumentation.
  void instrumentBasicBlockAssembly(BasicBlock& BB);
  
  // Omnifuzz's requests on instrumentation.
  std::unique_ptr<omnifuzz::FeedbackMechanism> fdbk_mech_;

  // Global Variables for operations
  GlobalVariable* SetupFailGV; 
  GlobalVariable* ShmPtrGV;

  // The Forkserver function programmed in this pass.
  Function* ForkserverInitFunction;
};

} // namespace llvm


#endif  // OMNIFUZZ_INSTRUMENTATION_H
