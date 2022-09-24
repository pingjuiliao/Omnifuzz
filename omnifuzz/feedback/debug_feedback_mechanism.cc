
#include "omnifuzz/feedback/debug_feedback_mechanism.h"

namespace omnifuzz {

DebugFeedbackMechanism::DebugFeedbackMechanism() {}

DebugFeedbackMechanism::~DebugFeedbackMechanism() {}

bool DebugFeedbackMechanism::DeemInteresting(void) {
  return false;
}

void DebugFeedbackMechanism::RegisterFeedbackData(void) {}

void DebugFeedbackMechanism::WriteOnBasicBlock(std::string& assembly) {
  std::stringstream ss;
  ss << "push %rax\n"
     << "add $$0x1337, %rax\n"
     << "pop %rax\n";
  assembly += ss.str();
}


  
} // namespace omnifuzz
