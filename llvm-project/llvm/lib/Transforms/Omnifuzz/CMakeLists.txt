add_llvm_component_library(LLVMOmnifuzz
  OmnifuzzInstrumentation.cpp
  ${OMNIFUZZ_SRC_DIR}/omnifuzz/feedback/afl_mechanism.cc
  ${OMNIFUZZ_SRC_DIR}/omnifuzz/feedback/debug_mechanism.cc

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/Omnifuzz
  ${OMNIFUZZ_INCLUDE_DIR}/omnifuzz/feedback

  DEPENDS
  intrinsics_gen

  LINK_COMPONENTS
  Analysis
  Core
  Support
)

target_include_directories(LLVMOmnifuzz PUBLIC ${OMNIFUZZ_INCLUDE_DIR})

