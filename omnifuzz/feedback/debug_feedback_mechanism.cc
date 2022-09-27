
#include "omnifuzz/feedback/debug_feedback_mechanism.h"


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

FuzzScore DebugFeedbackMechanism::DeemInteresting(void *p) {
  return FuzzScore::kRevisitCoverage;
}

void DebugFeedbackMechanism::WriteOnBasicBlock(std::string &s) {}

} // namespace omnifuzz

