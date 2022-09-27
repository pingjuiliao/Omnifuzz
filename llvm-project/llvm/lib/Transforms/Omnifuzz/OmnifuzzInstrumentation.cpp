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
    if (F.getName() == "omnifuzz_init_forksrv") {
      continue;
    }
    for (auto &BB: F) {
      if (BB.getName().startswith("__omnifuzz")) {
        continue;
      }
      instrumentBasicBlockAssembly(BB);
    }
  }
  return PreservedAnalyses::all();
}


bool OmnifuzzPass::initialize(Module &M) {
  
  LLVMContext &C = M.getContext();
  
  IntegerType *Int8Ty = IntegerType::getInt8Ty(C);
  IntegerType *Int32Ty = IntegerType::getInt32Ty(C);

  // Init Embedded forkserver
  InitGV = new GlobalVariable(M, Int32Ty, false, GlobalValue::ExternalLinkage, ConstantInt::get(Int32Ty, 0), "omnifuzz_has_inited"); 
   
  FailGV = new GlobalVariable(M, Int32Ty, false, GlobalValue::ExternalLinkage, ConstantInt::get(Int32Ty, 0), "__omnifuzz_failure"); 

  ShmPtrGV = new GlobalVariable(M, PointerType::getInt8PtrTy(C), 
      false, GlobalValue::ExternalLinkage, ConstantPointerNull::get(PointerType::getInt8PtrTy(C)), "__omnifuzz_shm_ptr");
  

  // Request
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

  return true;
}


void OmnifuzzPass::instrumentEmbeddedForkserver(BasicBlock* BB, 
                                                BasicBlock* UpdateBB, 
                                                BasicBlock* RetBB) {
  Module* M = BB->getModule(); 
  LLVMContext& C = BB->getContext();
  IntegerType *Int32Ty = IntegerType::getInt32Ty(C);
  PointerType *Ptr8Ty = PointerType::getInt8PtrTy(C);
  ConstantPointerNull* NullPtr8 = ConstantPointerNull::get(Ptr8Ty);
  
  FunctionCallee GetenvFunc = M->getOrInsertFunction("getenv", FunctionType::get(PointerType::getInt8PtrTy(C), {PointerType::getInt8PtrTy(C)}, false));
  FunctionCallee AtoiFunc = M->getOrInsertFunction("atoi", FunctionType::get(Int32Ty, {PointerType::getInt8PtrTy(C)}, false));
  FunctionCallee CloseFunc = M->getOrInsertFunction("close", FunctionType::get(Int32Ty, {Int32Ty}, false));
  FunctionCallee ExitFunc = M->getOrInsertFunction("exit", FunctionType::get(Type::getVoidTy(C), {Int32Ty}, false));
  FunctionCallee ForkFunc = M->getOrInsertFunction("fork", FunctionType::get(Int32Ty, {}, false));
  FunctionCallee PutsFunc = M->getOrInsertFunction("puts", FunctionType::get(Int32Ty, {Ptr8Ty}, false));
  FunctionCallee ReadFunc = M->getOrInsertFunction("read", FunctionType::get(Int32Ty, {Int32Ty, Ptr8Ty, Int32Ty}, false));
  FunctionCallee ShmatFunc = M->getOrInsertFunction("shmat", FunctionType::get(Ptr8Ty, {Int32Ty, Ptr8Ty, Int32Ty}, false));
  FunctionCallee WaitpidFunc = M->getOrInsertFunction("waitpid", FunctionType::get(Int32Ty, {Int32Ty, PointerType::get(Int32Ty, 0), Int32Ty}, false));
  FunctionCallee WriteFunc = M->getOrInsertFunction("write", FunctionType::get(Int32Ty, {Int32Ty, Ptr8Ty, Int32Ty}, false));
  
  // Basicblock spliting
  // FailChkBB ---> GetenvBB ---> ForksrvInitBB ---> ForksrvParentBB ---> 
  //  |
  //  v
  // AbortBB
  BasicBlock* FailChkBB = BB;
  BasicBlock* GetenvBB = BB->splitBasicBlock(BB->getTerminator(), "__omnifuzz_getenv_shm");
  BasicBlock* ForksrvInitBB = GetenvBB->splitBasicBlock(GetenvBB->getTerminator(), 
                                                        "__omnifuzz_forksrv_init");
  BasicBlock* ForksrvWaitBB = ForksrvInitBB->splitBasicBlock(ForksrvInitBB->getTerminator(), 
                                                        "__omnifuzz_forksrv_wait");
  BasicBlock* ForksrvExitBB = ForksrvWaitBB->splitBasicBlock(ForksrvWaitBB->getTerminator(), 
                                                        "__omnifuzz_forksrv_exit");
  BasicBlock* ForksrvSplitBB = ForksrvExitBB->splitBasicBlock(ForksrvExitBB->getTerminator(), 
                                                        "__omnifuzz_forksrv_split");
  BasicBlock* ForksrvParentBB = ForksrvSplitBB->splitBasicBlock(ForksrvSplitBB->getTerminator(), 
                                                        "__omnifuzz_forksrv_parent");
  BasicBlock* ForksrvResumeBB = ForksrvParentBB->splitBasicBlock(ForksrvParentBB->getTerminator(), 
                                                        "__omnifuzz_forksrv_resume");
  BasicBlock* AbortBB = ForksrvResumeBB->splitBasicBlock(ForksrvResumeBB->getTerminator(), 
                                                         "__omnifuzz_forksrv_wait");
  
  // If failure flag is on, return to normal execution
  IRBuilder<> FailChkIRB(FailChkBB->getTerminator());
  LoadInst* LdFailGV = FailChkIRB.CreateLoad(Int32Ty, FailGV);
  Value* TestFailGV = FailChkIRB.CreateICmpNE(LdFailGV, FailChkIRB.getInt32(0));
  FailChkIRB.CreateCondBr(TestFailGV, RetBB, GetenvBB);
  FailChkBB->getTerminator()->eraseFromParent();
  
  // The system factor to decide whether we are on forkserver: getenv("OMNIFUZZ_SHM_ENV");
  IRBuilder<> GetenvIRB(GetenvBB->getTerminator());
  Constant* ShmStr = GetenvIRB.CreateGlobalStringPtr("OMNIFUZZ_SHM_ENV");
  CallInst* GetenvRetVal = GetenvIRB.CreateCall(GetenvFunc, {ShmStr});
  Value* TestGetenvRetVal = GetenvIRB.CreateICmpNE(GetenvRetVal, ConstantPointerNull::get(PointerType::getInt8PtrTy(C)));
  GetenvIRB.CreateCondBr(TestGetenvRetVal, ForksrvInitBB, AbortBB);
  GetenvBB->getTerminator()->eraseFromParent(); 
  
  // Init forkserver
  IRBuilder<> ForksrvInitIRB(ForksrvInitBB->getTerminator());
  Value* AtoiRetVal = ForksrvInitIRB.CreateCall(AtoiFunc, {GetenvRetVal});
  Value* ShmatRetVal = ForksrvInitIRB.CreateCall(ShmatFunc, {AtoiRetVal, NullPtr8, ForksrvInitIRB.getInt32(0)});
  ForksrvInitIRB.CreateStore(ShmatRetVal, ShmPtrGV);
  Value* InitStr = ForksrvInitIRB.CreateGlobalStringPtr("init");
  Value* WriteInitCodeCall = ForksrvInitIRB.CreateCall(WriteFunc, {ForksrvInitIRB.getInt32(21), InitStr, ForksrvInitIRB.getInt32(4)});
  Value* WriteInitCodeRetVal = ForksrvInitIRB.CreateICmpUGE(WriteInitCodeCall, ForksrvInitIRB.getInt32(4));
  ForksrvInitIRB.CreateCondBr(WriteInitCodeRetVal, ForksrvWaitBB, AbortBB);
  ForksrvInitBB->getTerminator()->eraseFromParent();
  
  // ForksrvWait: The start of the forkserver main loop
  IRBuilder<> ForksrvWaitIRB(ForksrvWaitBB->getTerminator());
  AllocaInst* PidAlloca = ForksrvWaitIRB.CreateAlloca(Int32Ty);
  Value* InitGVBitCast = ForksrvWaitIRB.CreateBitCast(InitGV, Ptr8Ty);
  ForksrvWaitIRB.CreateCall(ReadFunc, {ForksrvInitIRB.getInt32(22), InitGVBitCast, ForksrvInitIRB.getInt32(4)});
  Value* ForkRetVal = ForksrvWaitIRB.CreateCall(ForkFunc, {});
  ForksrvWaitIRB.CreateStore(ForkRetVal, PidAlloca); 
  Value* ForkFailed = ForksrvWaitIRB.CreateICmpSLT(ForkRetVal, ForksrvWaitIRB.getInt32(0)); 
  ForksrvWaitIRB.CreateCondBr(ForkFailed, ForksrvExitBB, ForksrvSplitBB);
  ForksrvWaitBB->getTerminator()->eraseFromParent();

  // ForksrvExit: if (pid < 0)
  IRBuilder<> ForksrvExitIRB(ForksrvExitBB->getTerminator());
  Constant* ExitMessage = ForksrvExitIRB.CreateGlobalStringPtr("Failed on fork()\n");
  ForksrvExitIRB.CreateCall(PutsFunc, {ExitMessage});
  ForksrvExitIRB.CreateCall(ExitFunc, {ForksrvExitIRB.getInt32(-1)}); 
  ForksrvExitIRB.CreateUnreachable();
  ForksrvExitBB->getTerminator()->eraseFromParent();

  // ForksrvSplit: determine whether (pid == 0) or (pid == child_pid)
  IRBuilder<> ForksrvSplitIRB(ForksrvSplitBB->getTerminator());
  Value* ForkPidIsParent = ForksrvSplitIRB.CreateICmpNE(ForkRetVal, ForksrvSplitIRB.getInt32(0));
  ForksrvSplitIRB.CreateCondBr(ForkPidIsParent, ForksrvParentBB, ForksrvResumeBB);
  ForksrvSplitBB->getTerminator()->eraseFromParent();

  // ForksrvParent
  IRBuilder<> ForksrvParentIRB(ForksrvParentBB->getTerminator());
  Value* BitCastPid = ForksrvParentIRB.CreateBitCast(PidAlloca, Ptr8Ty);
  ForksrvParentIRB.CreateCall(WriteFunc, {ForksrvParentIRB.getInt32(22), 
                                          BitCastPid, 
                                          ForksrvParentIRB.getInt32(4)});
  AllocaInst* ChildStatus = ForksrvParentIRB.CreateAlloca(Int32Ty);
  ForksrvParentIRB.CreateCall(WaitpidFunc, {ForkRetVal, ChildStatus, ForksrvParentIRB.getInt32(0)});
  Value* BitCastStatus = ForksrvParentIRB.CreateBitCast(ChildStatus, Ptr8Ty);
  ForksrvParentIRB.CreateCall(WriteFunc, {ForksrvParentIRB.getInt32(22), BitCastStatus,ForksrvParentIRB.getInt32(4)});
  ForksrvParentIRB.CreateBr(ForksrvWaitBB);
  ForksrvParentBB->getTerminator()->eraseFromParent(); 
  
  // ForksrvResume (ForksrvChild)
  IRBuilder<> ForksrvResumeIRB(ForksrvResumeBB->getTerminator());
  ForksrvResumeIRB.CreateCall(CloseFunc, {ForksrvResumeIRB.getInt32(21)});
  ForksrvResumeIRB.CreateCall(CloseFunc, {ForksrvResumeIRB.getInt32(21)});
  ForksrvResumeIRB.CreateBr(UpdateBB);
  ForksrvResumeBB->getTerminator()->eraseFromParent(); 

  // Abort: turn failure flag on, return to normal execution. 
  IRBuilder<> AbortIRB(AbortBB->getTerminator());
  LoadInst* LdFailGV2 = AbortIRB.CreateLoad(Int32Ty, FailGV);
  Value* AddedFail = AbortIRB.CreateAdd(LdFailGV2, AbortIRB.getInt32(0x1));
  AbortIRB.CreateStore(AddedFail, FailGV);
  AbortIRB.CreateBr(RetBB);
  AbortBB->getTerminator()->eraseFromParent();
}



void OmnifuzzPass::instrumentBasicBlockAssembly(BasicBlock& BB) {

  LLVMContext& C = BB.getContext();
  PointerType *Ptr8Ty = PointerType::getInt8PtrTy(C);
  ConstantPointerNull* NullPtr8 = ConstantPointerNull::get(Ptr8Ty);

  // split basicblocks
  BasicBlock* CheckBB = &BB;
  BasicBlock* SetupBB = CheckBB->splitBasicBlock(CheckBB->getTerminator(), 
                            "__omnifuzz_SetupBB");
  BasicBlock* UpdateBB = SetupBB->splitBasicBlock(SetupBB->getTerminator(), 
                            "__omnifuzz_UpdateBB");
  BasicBlock* RetBB = UpdateBB->splitBasicBlock(UpdateBB->getTerminator(), 
                            "__omnifuzz_RetBB");
  
  // Check if SHM pointer exists.
  IRBuilder<> CheckIRB(CheckBB->getTerminator());
  LoadInst* LdShmPtrGV = CheckIRB.CreateLoad(Ptr8Ty, ShmPtrGV);
  Value* TestShmPtrGV = CheckIRB.CreateICmpEQ(LdShmPtrGV, NullPtr8);
  CheckIRB.CreateCondBr(TestShmPtrGV, SetupBB, UpdateBB);
  CheckBB->getTerminator()->eraseFromParent();

  // SetupBB
  instrumentEmbeddedForkserver(SetupBB, UpdateBB, RetBB);

  // update BB
  IRBuilder<> UpdateIRB(UpdateBB->getTerminator());
  std::string AsmStr;
  fdbk_mech_->WriteOnBasicBlock(AsmStr);
  InlineAsm *BlockAsm = InlineAsm::get(
      FunctionType::get(UpdateIRB.getVoidTy(), {}, false), AsmStr, 
      "", false);
  UpdateIRB.CreateCall(BlockAsm, {});
}


} // namespace llvm
