add_llvm_component_library(LLVMOmnifuzz
  OmnifuzzInstrumentation.cpp
	debug_feedback.cc

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/Omnifuzz

  DEPENDS
  intrinsics_gen

  LINK_COMPONENTS
  Analysis
  Core
  Support
)

