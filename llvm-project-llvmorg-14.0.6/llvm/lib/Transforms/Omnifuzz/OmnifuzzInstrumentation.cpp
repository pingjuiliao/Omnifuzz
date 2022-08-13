#include "llvm/Transforms/Omnifuzz/OmnifuzzInstrumentation.h"
using namespace llvm;

PreservedAnalyses OmnifuzzInstrumentationPass::run(Function &F, FunctionAnalysisManager &AM) {
  static bool initialized = false;
  Module* M = F.getParent();
  if (!initialized) {
    initialized = initialization(M);
  }
  errs() << F.getName() << "\n";
  for (auto &BB: F) {
    instrumentOn(BB);
  }
  
  return PreservedAnalyses::all();
}


void OmnifuzzInstrumentationPass::instrumentOn(BasicBlock& BB) {
  Function* F = BB.getParent();
  Module* M = F->getParent();
  IRBuilder<> IRB(BB.getFirstNonPHI());
  /*
  Function* PutsFunc = M->getFunction("puts");
  GlobalVariable *fname = IRB.CreateGlobalString(F->getName());
  IRB.CreateCall(PutsFunc->getFunctionType(), PutsFunc, {fname});
  */
}

bool OmnifuzzInstrumentationPass::initialization(Module *M) {
  // errs() << "Initialized\n"; 
  // LLVMContext &C = M->getContext();


  return true;
}
