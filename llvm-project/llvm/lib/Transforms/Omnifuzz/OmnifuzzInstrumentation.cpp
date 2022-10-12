#include "llvm/Transforms/Omnifuzz/OmnifuzzInstrumentation.h"

namespace llvm {

OmnifuzzPass::OmnifuzzPass(std::unique_ptr<omnifuzz::FeedbackMechanism> feedback_mechanism) {
  fdbk_mech_ = std::move(feedback_mechanism);
  ForkserverInitFunction = nullptr;
}

PreservedAnalyses OmnifuzzPass::run(Module& M, ModuleAnalysisManager& AM) {
  
  static bool inited = false;
  
  // No instrumentation if there's no feedback mechanism
  if (!fdbk_mech_) {
    return PreservedAnalyses::all();
  }

  // Prepare forkserver, SHM, and others
  if (!inited) {
    inited = performDeclaration(M);
    createForkserverFunction(M);
  } 
  
  for (auto &F: M) {
    if (F.getName() == ForkserverInitFunction->getName()) {
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

void OmnifuzzPass::createForkserverFunction(Module& M) {
  
  LLVMContext& C = M.getContext();
  
  IntegerType *Int32Ty = IntegerType::getInt32Ty(C);
  PointerType *Ptr8Ty = PointerType::getInt8PtrTy(C);
  ConstantPointerNull* NullPtr8 = ConstantPointerNull::get(Ptr8Ty);

  // Used Functions
  FunctionCallee GetenvFunc = M.getOrInsertFunction("getenv", FunctionType::get(PointerType::getInt8PtrTy(C), {PointerType::getInt8PtrTy(C)}, false));
  FunctionCallee AtoiFunc = M.getOrInsertFunction("atoi", FunctionType::get(Int32Ty, {PointerType::getInt8PtrTy(C)}, false));
  FunctionCallee CloseFunc = M.getOrInsertFunction("close", FunctionType::get(Int32Ty, {Int32Ty}, false));
  FunctionCallee ExitFunc = M.getOrInsertFunction("exit", FunctionType::get(Type::getVoidTy(C), {Int32Ty}, false));
  FunctionCallee ForkFunc = M.getOrInsertFunction("fork", FunctionType::get(Int32Ty, {}, false));
  FunctionCallee PrintfFunc = M.getOrInsertFunction("printf", FunctionType::get(Int32Ty, {Ptr8Ty}, true));
  FunctionCallee ReadFunc = M.getOrInsertFunction("read", FunctionType::get(Int32Ty, {Int32Ty, Ptr8Ty, Int32Ty}, false));
  FunctionCallee ShmatFunc = M.getOrInsertFunction("shmat", FunctionType::get(Ptr8Ty, {Int32Ty, Ptr8Ty, Int32Ty}, false));
  FunctionCallee WaitpidFunc = M.getOrInsertFunction("waitpid", FunctionType::get(Int32Ty, {Int32Ty, PointerType::get(Int32Ty, 0), Int32Ty}, false));
  FunctionCallee WriteFunc = M.getOrInsertFunction("write", FunctionType::get(Int32Ty, {Int32Ty, Ptr8Ty, Int32Ty}, false));
#ifdef OMNIFUZZ_DEBUG
  FunctionCallee GetpidFunc = M.getOrInsertFunction("getpid", FunctionType::get(Int32Ty, {}, false));
#endif

  // Function Creation
  FunctionType* FnTy = FunctionType::get(Type::getVoidTy(C), 
                                         {}, false);
  FunctionCallee MyFnCallee = M.getOrInsertFunction(
      "__omnifuzz_init_forkserver", FnTy);
  ForkserverInitFunction = cast<Function>(MyFnCallee.getCallee());
  
  // Function Property
  // Fn->deleteBody();
  // Fn->setAlignment(Align(4096));

  BasicBlock* FailChkBB = BasicBlock::Create(C, "__omnifuzz_forksrv_failchk", ForkserverInitFunction);
  BasicBlock* GetenvBB = BasicBlock::Create(C, "__omnifuzz_forksrv_getenv", ForkserverInitFunction);
  BasicBlock* ForksrvInitBB = BasicBlock::Create(C, "__omnifuzz_forksrv_init", ForkserverInitFunction);
  BasicBlock* ForksrvWaitBB = BasicBlock::Create(C, "__omnifuzz_forksrv_wait", ForkserverInitFunction);
  BasicBlock* ForksrvExitBB = BasicBlock::Create(C, "__omnifuzz_forksrv_exit", ForkserverInitFunction);
  BasicBlock* ForksrvSplitBB = BasicBlock::Create(C, "__omnifuzz_forksrv_split", ForkserverInitFunction);
  BasicBlock* ForksrvParentBB = BasicBlock::Create(C, "__omnifuzz_forksrv_parent", ForkserverInitFunction);
  BasicBlock* ForksrvResumeBB = BasicBlock::Create(C, "__omnifuzz_forksrv_resume", ForkserverInitFunction);
  BasicBlock* AbortBB = BasicBlock::Create(C, "__omnifuzz_forksrv_abort", ForkserverInitFunction);
  BasicBlock* ReturnBB = BasicBlock::Create(C, "__omnifuzz_forksrv_return", ForkserverInitFunction);
  
  // If failure flag is on, return to normal execution
  IRBuilder<> FailChkIRB(FailChkBB);
  LoadInst* LdFailGV = FailChkIRB.CreateLoad(Int32Ty, SetupFailGV);
  Value* TestFailGV = FailChkIRB.CreateICmpNE(LdFailGV, FailChkIRB.getInt32(0));
  FailChkIRB.CreateCondBr(TestFailGV, ReturnBB, GetenvBB);
  
  // The system factor to decide whether we are on forkserver: getenv("OMNIFUZZ_SHM_ENV");
  IRBuilder<> GetenvIRB(GetenvBB);
  Constant* ShmStr = GetenvIRB.CreateGlobalStringPtr("OMNIFUZZ_SHM_ENV");
  CallInst* GetenvRetVal = GetenvIRB.CreateCall(GetenvFunc, {ShmStr});
  Value* TestGetenvRetVal = GetenvIRB.CreateICmpNE(GetenvRetVal, ConstantPointerNull::get(PointerType::getInt8PtrTy(C)));
  GetenvIRB.CreateCondBr(TestGetenvRetVal, ForksrvInitBB, AbortBB);
  
  // Init forkserver
  IRBuilder<> ForksrvInitIRB(ForksrvInitBB);
  Value* AtoiRetVal = ForksrvInitIRB.CreateCall(AtoiFunc, {GetenvRetVal});
  Value* ShmatRetVal = ForksrvInitIRB.CreateCall(ShmatFunc, {AtoiRetVal, NullPtr8, ForksrvInitIRB.getInt32(0)});
  ForksrvInitIRB.CreateStore(ShmatRetVal, ShmPtrGV);
  size_t ShmOffset = 0;
  for (auto data: fdbk_mech_->fdbk_data_map_) {
    LoadInst* LoadShm = ForksrvInitIRB.CreateLoad(Ptr8Ty, ShmPtrGV);
    Value* ShmAddrInteger = ForksrvInitIRB.CreatePtrToInt(
        LoadShm, ForksrvInitIRB.getInt64Ty());
    Value* AddedInteger = ForksrvInitIRB.CreateAdd(
        ShmAddrInteger, ForksrvInitIRB.getInt64(ShmOffset)); 
    Value* OffsettedPtr = ForksrvInitIRB.CreateIntToPtr(AddedInteger, Ptr8Ty);
    Value* FdbkDataGV = M.getNamedGlobal(data.second.GetName());
    ForksrvInitIRB.CreateStore(OffsettedPtr, FdbkDataGV);
    ShmOffset += data.second.GetSize(); 
  }
  Value* InitStr = ForksrvInitIRB.CreateGlobalStringPtr("init");
  Value* WriteInitCodeCall = ForksrvInitIRB.CreateCall(WriteFunc, {ForksrvInitIRB.getInt32(22), InitStr, ForksrvInitIRB.getInt32(4)});
  Value* WriteInitCodeRetVal = ForksrvInitIRB.CreateICmpUGE(WriteInitCodeCall, ForksrvInitIRB.getInt32(4));
  ForksrvInitIRB.CreateCondBr(WriteInitCodeRetVal, ForksrvWaitBB, AbortBB);
  
  // ForksrvWait: The start of the forkserver main loop
  IRBuilder<> ForksrvWaitIRB(ForksrvWaitBB);
#ifdef OMNIFUZZ_DEBUG
  // check if the server loop works perfectly
  Value* CallGetpid = ForksrvWaitIRB.CreateCall(GetpidFunc, {});
  Value* FormatStr = ForksrvWaitIRB.CreateGlobalStringPtr("ServerPID: %d\n");
  ForksrvWaitIRB.CreateCall(PrintfFunc, {FormatStr, CallGetpid});
#endif
  AllocaInst* PidAlloca = ForksrvWaitIRB.CreateAlloca(Int32Ty);
  AllocaInst* RequestCodeAlloca = ForksrvWaitIRB.CreateAlloca(Int32Ty);
  Value* RequestCodeBitCast = ForksrvWaitIRB.CreateBitCast(RequestCodeAlloca, Ptr8Ty);
  ForksrvWaitIRB.CreateCall(ReadFunc, {ForksrvInitIRB.getInt32(21), RequestCodeBitCast, ForksrvInitIRB.getInt32(4)});
  Value* ForkRetVal = ForksrvWaitIRB.CreateCall(ForkFunc, {});
  ForksrvWaitIRB.CreateStore(ForkRetVal, PidAlloca); 
  Value* ForkFailed = ForksrvWaitIRB.CreateICmpSLT(ForkRetVal, ForksrvWaitIRB.getInt32(0)); 
  ForksrvWaitIRB.CreateCondBr(ForkFailed, ForksrvExitBB, ForksrvSplitBB);

  // ForksrvExit: if (pid < 0)
  IRBuilder<> ForksrvExitIRB(ForksrvExitBB);
  Constant* ExitMessage = ForksrvExitIRB.CreateGlobalStringPtr("Failed on fork()\n");
  ForksrvExitIRB.CreateCall(PrintfFunc, {ExitMessage});
  ForksrvExitIRB.CreateCall(ExitFunc, {ForksrvExitIRB.getInt32(-1)}); 
  ForksrvExitIRB.CreateUnreachable();

  // ForksrvSplit: split child/parent
  IRBuilder<> ForksrvSplitIRB(ForksrvSplitBB);
  Value* ForkPidIsParent = ForksrvSplitIRB.CreateICmpNE(ForkRetVal, ForksrvSplitIRB.getInt32(0));
  ForksrvSplitIRB.CreateCondBr(ForkPidIsParent, ForksrvParentBB, ForksrvResumeBB);

  // ForksrvParent
  IRBuilder<> ForksrvParentIRB(ForksrvParentBB);
  Value* BitCastPid = ForksrvParentIRB.CreateBitCast(PidAlloca, Ptr8Ty);
  ForksrvParentIRB.CreateCall(WriteFunc, {ForksrvParentIRB.getInt32(22), 
                                          BitCastPid, 
                                          ForksrvParentIRB.getInt32(4)});
  AllocaInst* ChildStatus = ForksrvParentIRB.CreateAlloca(Int32Ty);
  ForksrvParentIRB.CreateCall(WaitpidFunc, {ForkRetVal, ChildStatus, ForksrvParentIRB.getInt32(0)});
  Value* BitCastStatus = ForksrvParentIRB.CreateBitCast(ChildStatus, Ptr8Ty);
  ForksrvParentIRB.CreateCall(WriteFunc, {ForksrvParentIRB.getInt32(22), BitCastStatus,ForksrvParentIRB.getInt32(4)});
  ForksrvParentIRB.CreateBr(ForksrvWaitBB);
  
  // ForksrvResume (ForksrvChild)
  IRBuilder<> ForksrvResumeIRB(ForksrvResumeBB);
#ifdef OMNIFUZZ_DEBUG
  Value* Flag = ForksrvResumeIRB.CreateGlobalStringPtr("__omnifuzz_forksrv_resume"); 
  ForksrvResumeIRB.CreateCall(PrintfFunc, {Flag});
#endif
  ForksrvResumeIRB.CreateCall(CloseFunc, {ForksrvResumeIRB.getInt32(21)});
  ForksrvResumeIRB.CreateCall(CloseFunc, {ForksrvResumeIRB.getInt32(22)});
  std::string AsmStr;
  fdbk_mech_->WriteOnBasicBlock(AsmStr);
  InlineAsm *BlockAsm = InlineAsm::get(
      FunctionType::get(ForksrvResumeIRB.getVoidTy(), {}, false), AsmStr, 
      "", false);
  ForksrvResumeIRB.CreateCall(BlockAsm, {});
  ForksrvResumeIRB.CreateBr(ReturnBB);

  // Abort: turn failure flag on, return to normal execution. 
  IRBuilder<> AbortIRB(AbortBB);
  LoadInst* LdFailGV2 = AbortIRB.CreateLoad(Int32Ty, SetupFailGV);
  Value* AddedFail = AbortIRB.CreateAdd(LdFailGV2, AbortIRB.getInt32(0x1));
  AbortIRB.CreateStore(AddedFail, SetupFailGV);
  AbortIRB.CreateBr(ReturnBB);
  
  // Must-have this block, otherwise, llvm will fail
  IRBuilder<> ReturnIRB(ReturnBB);
  ReturnIRB.CreateRetVoid();
}



bool OmnifuzzPass::performDeclaration(Module &M) {
  
  LLVMContext &C = M.getContext();
  
  IntegerType *Int8Ty = IntegerType::getInt8Ty(C);
  IntegerType *Int32Ty = IntegerType::getInt32Ty(C);

  // Init Embedded forkserver
  SetupFailGV = new GlobalVariable(M, Int32Ty, false, GlobalValue::ExternalLinkage, ConstantInt::get(Int32Ty, 0), "__omnifuzz_failure"); 

  ShmPtrGV = new GlobalVariable(M, PointerType::getInt8PtrTy(C), 
      false, GlobalValue::ExternalLinkage, ConstantPointerNull::get(PointerType::getInt8PtrTy(C)), "__omnifuzz_shm_ptr");
  

  // Execution Variable: some temporary variable that operates data
  for (auto exec_var: fdbk_mech_->exec_var_map_) {
    Type* type;
    Constant* InitValue;
    switch(exec_var.second.GetType()) {
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
        InitValue, exec_var.second.GetName());
    errs() << "Exec Var: "  << exec_var.second.GetName() << " " << GV << "\n";
  }
  
  // Register Feedback data (later be initialized on SHM) 
  for (auto data: fdbk_mech_->fdbk_data_map_) {
    if (data.second.GetType() != omnifuzz::ExecutionVariable::Pointer) {
      errs() << "[Error] Cannot support non-pointer type of feedback data\n";
      return false;
    }
    PointerType* PtrType = PointerType::get(Int8Ty, 0);
    ConstantPointerNull* NullPtr = ConstantPointerNull::get(PtrType);
    GlobalVariable* GV = new GlobalVariable(M, PtrType, false, 
        GlobalValue::ExternalLinkage, NullPtr, data.second.GetName());
    errs() << "Fdbk Data: " << data.second.GetName() << " " << GV << "\n";
  }

  return true;
}

void OmnifuzzPass::instrumentBasicBlockAssembly(BasicBlock& BB) {

  LLVMContext& C = BB.getContext();
  PointerType *Ptr8Ty = PointerType::getInt8PtrTy(C);
  ConstantPointerNull* NullPtr8 = ConstantPointerNull::get(Ptr8Ty);

  // split basicblocks
  BasicBlock* CheckBB = &BB;
  BasicBlock* SetupBB = CheckBB->splitBasicBlock(CheckBB->getFirstInsertionPt(), 
                            "__omnifuzz_SetupBB");
  BasicBlock* UpdateBB = SetupBB->splitBasicBlock(SetupBB->getFirstInsertionPt(), 
                            "__omnifuzz_UpdateBB");
  BasicBlock* RetBB = UpdateBB->splitBasicBlock(UpdateBB->getFirstInsertionPt(), 
                            "__omnifuzz_RetBB");
  
  // Check if SHM pointer exists.
  IRBuilder<> CheckIRB(CheckBB->getTerminator());
  LoadInst* LdShmPtrGV = CheckIRB.CreateLoad(Ptr8Ty, ShmPtrGV);
  Value* TestShmPtrGV = CheckIRB.CreateICmpEQ(LdShmPtrGV, NullPtr8);
  CheckIRB.CreateCondBr(TestShmPtrGV, SetupBB, UpdateBB);
  CheckBB->getTerminator()->eraseFromParent();

  // SetupBB
  // instrumentEmbeddedForkserver(SetupBB, UpdateBB, RetBB);
  IRBuilder<> SetupIRB(SetupBB->getTerminator());
  SetupIRB.CreateCall(ForkserverInitFunction, {});
  SetupIRB.CreateBr(RetBB);
  SetupBB->getTerminator()->eraseFromParent();

  // update BB
  IRBuilder<> UpdateIRB(UpdateBB->getTerminator());
  std::string AsmStr;
#ifdef AFL_DEBUG
  Module* M = BB.getModule();
  IntegerType *Int64Ty = IntegerType::getInt64Ty(C);
  IntegerType *Int32Ty = IntegerType::getInt32Ty(C);
  FunctionCallee PrintfFunc = BB.getModule()->getOrInsertFunction("printf", FunctionType::get(Int32Ty, {Ptr8Ty}, true));
  Value* FormatBeforeUpdate = UpdateIRB.CreateGlobalStringPtr("__afl_prev_loc: %016x\t");
  GlobalVariable* AflPrevLoc = M->getGlobalVariable("__afl_prev_loc");
  LoadInst* LoadPrevLoc = UpdateIRB.CreateLoad(Int64Ty, AflPrevLoc);
  UpdateIRB.CreateCall(PrintfFunc, {FormatBeforeUpdate, LoadPrevLoc});
#endif

  fdbk_mech_->WriteOnBasicBlock(AsmStr);
  InlineAsm *BlockAsm = InlineAsm::get(
      FunctionType::get(UpdateIRB.getVoidTy(), {}, false), AsmStr, 
      "", false);
  UpdateIRB.CreateCall(BlockAsm, {});
#ifdef AFL_DEBUG
  Value* FormatAfterUpdate = UpdateIRB.CreateGlobalStringPtr(", after: %016x\n");
  LoadInst* LoadPrevLocAfter = UpdateIRB.CreateLoad(Int64Ty, AflPrevLoc);
  UpdateIRB.CreateCall(PrintfFunc, {FormatAfterUpdate, LoadPrevLocAfter});
#endif
}

} // namespace llvm
