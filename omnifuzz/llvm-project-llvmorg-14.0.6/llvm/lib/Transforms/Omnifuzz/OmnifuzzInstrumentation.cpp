#include "llvm/Transforms/Omnifuzz/OmnifuzzInstrumentation.h"

namespace llvm {

OmnifuzzPass::OmnifuzzPass(std::unique_ptr<omnifuzz::Feedback> feedback) {
  feedback_ = std::move(feedback);
}

PreservedAnalyses OmnifuzzPass::run(Module& M, ModuleAnalysisManager& AM) {
  
  static bool inited = false;

  if (!feedback_) {
    return PreservedAnalyses::all();
  }

  if (!inited) {
    inited = initialize(M);
  } 
  
  for (auto &F: M) {
    for (auto &BB: F) {
      instrumentsBasicBlockAssembly(BB);
    }
  }
  return PreservedAnalyses::all();
}


bool OmnifuzzPass::initialize(Module &M) {
  
  LLVMContext &C = M.getContext();
  
  IntegerType *Int8Ty = IntegerType::getInt8Ty(C);
  IntegerType *Int32Ty = IntegerType::getInt32Ty(C);
  ConstantInt *IntZero = ConstantInt::get(Int32Ty, 0);

  GlobalVariable* AflMapPtr = 
    new GlobalVariable(M, PointerType::get(Int8Ty, 0), false, 
                       GlobalValue::ExternalLinkage, 0, "__afl_area_ptr");
  GlobalVariable* AflPrevLoc = new GlobalVariable(
      M, Int32Ty, false, GlobalValue::ExternalLinkage, 
      IntZero, "__afl_prev_loc");
  
  for (auto &F: M) {
    for (auto &BB: F) {
      BasicBlock::iterator IP = BB.getFirstInsertionPt();
      IRBuilder<> IRB(&(*IP));

      LoadInst* counter = IRB.CreateLoad(Int32Ty, AflPrevLoc);
      Value* added = IRB.CreateAdd(counter, IRB.getInt32(0xda));
      IRB.CreateStore(added, AflPrevLoc);
    }
  }

  return true;
}




void OmnifuzzPass::instrumentsBasicBlockAssembly(BasicBlock& BB) {
  IRBuilder<> IRB(BB.getFirstNonPHI());
  std::string AsmStr;
  feedback_->WriteOnBasicBlock(AsmStr);
  /*AsmPerBlock << "push %rax\n"
              << "add $$0x1, %rax\n"
              << "pop %rax\n";
  */
  InlineAsm *BlockAsm = InlineAsm::get(
      FunctionType::get(IRB.getVoidTy(), {}, false), AsmStr, 
      "", false);
  IRB.CreateCall(BlockAsm, {});
}


} // namespace llvm
