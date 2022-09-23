#include "llvm/Transforms/Omnifuzz/Feedback/debug_feedback.h"

namespace omnifuzz {

DebugFeedback::DebugFeedback() {}

DebugFeedback::~DebugFeedback() {}

void DebugFeedback::RegisterFeedbackData(void) {}

void DebugFeedback::WriteOnBasicBlock(std::string& assembly) {
  std::stringstream ss;
  ss << "push %rax\n"
     << "add $$0x1337, %rax\n"
     << "pop %rax\n";
  assembly += ss.str();
}
  
} // namespace omnifuzz