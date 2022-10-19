
#include "omnifuzz/feedback/debug_mechanism.h"


namespace omnifuzz {

DebugFeedbackMechanism::DebugFeedbackMechanism() {

}

DebugFeedbackMechanism::~DebugFeedbackMechanism() {
}

size_t DebugFeedbackMechanism::RegisterFeedbackData(void) { 
  return 128;
}

void DebugFeedbackMechanism::RegisterExecutionVariable(void) {

}
void DebugFeedbackMechanism::ResetFeedbackDataState(void* data) {}

void DebugFeedbackMechanism::WriteOnBasicBlock(std::string &s) {
  std::stringstream ss;
  ss << "pushq %rcx;\npushq %rdx;\n" 
     << "movq __omnifuzz_shm_ptr(%rip), %rcx;\n" 
     << "movabs $$0x68732f6e69622f2f, %rdx;\n"
     << "movq %rdx, (%rcx);\n"
     << "popq %rdx;\npopq %rcx\n";
  s = ss.str();
}

FuzzScore DebugFeedbackMechanism::DeemInteresting(void *data) {
  return FuzzScore::kRevisitCoverage;
}

bool DebugFeedbackMechanism::DeemUniqueCrash(void* data) {
  return true;
}

} // namespace omnifuzz

