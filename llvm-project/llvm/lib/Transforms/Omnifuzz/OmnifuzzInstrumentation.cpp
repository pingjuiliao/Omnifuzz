#include "llvm/Transforms/Omnifuzz/OmnifuzzInstrumentation.h"

namespace llvm {

OmnifuzzPass::OmnifuzzPass(std::unique_ptr<omnifuzz::FeedbackMechanism> feedback_mechanism) {
  fdbk_mech_ = std::move(feedback_mechanism);
}

PreservedAnalyses OmnifuzzPass::run(Module& M, ModuleAnalysisManager& AM) {
  
  static bool inited = false;

  if (!fdbk_mech_) {
    return PreservedAnalyses::all();
  }

  if (!inited) {
    inited = initialize(M);
  } 
  
  for (auto &F: M) {
    for (auto &BB: F) {
      instrumentBasicBlockAssembly(BB);
    }
  }
  return PreservedAnalyses::all();
}


bool OmnifuzzPass::initialize(Module &M) {
  
  LLVMContext &C = M.getContext();
  
  IntegerType *Int8Ty = IntegerType::getInt8Ty(C);
  IntegerType *Int32Ty = IntegerType::getInt32Ty(C);
  // ConstantInt *IntZero = ConstantInt::get(Int32Ty, 0);
  /*
  GlobalVariable* AflMapPtr = 
    new GlobalVariable(M, PointerType::get(Int8Ty, 0), false, 
                       GlobalValue::ExternalLinkage, 0, "__afl_area_ptr");
  GlobalVariable* AflPrevLoc = new GlobalVariable(
      M, Int32Ty, false, GlobalValue::ExternalLinkage, 
      IntZero, "__afl_prev_loc");
  */
  for (auto data: fdbk_mech_->exec_var_map_) {
    Type* type;
    Constant* InitValue;
    switch(data.second.GetType()) {
      case omnifuzz::ExecutionVariable::Type::Int64:
        type = IntegerType::getInt64Ty(C);
        InitValue = ConstantInt::get(type, 0);
        break;
      case omnifuzz::ExecutionVariable::Type::Int32:
        type = Int32Ty;
        InitValue = ConstantInt::get(type, 0);
        break;
      case omnifuzz::ExecutionVariable::Type::Int16:
        type = IntegerType::getInt16Ty(C);
        InitValue = ConstantInt::get(type, 0);
        break;
      case omnifuzz::ExecutionVariable::Type::Int8:
        type = Int8Ty;
        InitValue = ConstantInt::get(type, 0);
        break;
      case omnifuzz::ExecutionVariable::Type::Pointer:
        type = PointerType::get(Int8Ty, 0);
        InitValue = ConstantPointerNull::get(static_cast<PointerType*>(type)); 
        break;

    }

    GlobalVariable* GV = new GlobalVariable(M, type, false, GlobalValue::ExternalLinkage, 
        InitValue, data.second.GetName());
    errs() << "GV: "  << data.second.GetName() << " " << GV << "\n";
  }

  
  for (auto &F: M) {
    for (auto &BB: F) {
      BasicBlock::iterator IP = BB.getFirstInsertionPt();
      IRBuilder<> IRB(&(*IP));
      
      GlobalVariable* AflPrevLoc = M.getNamedGlobal("__afl_prev_loc");
      if (!AflPrevLoc) 
        break;
      LoadInst* counter = IRB.CreateLoad(Int32Ty, AflPrevLoc);
      Value* added = IRB.CreateAdd(counter, IRB.getInt32(0xda));
      IRB.CreateStore(added, AflPrevLoc);
    }
  }

  return true;
}




void OmnifuzzPass::instrumentBasicBlockAssembly(BasicBlock& BB) {
  IRBuilder<> IRB(BB.getFirstNonPHI());
  std::string AsmStr;
  fdbk_mech_->WriteOnBasicBlock(AsmStr);
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
