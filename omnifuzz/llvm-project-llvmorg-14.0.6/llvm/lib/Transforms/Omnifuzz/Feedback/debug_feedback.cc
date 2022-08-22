#include "llvm/Transforms/Omnifuzz/Feedback/debug_feedback.h"

namespace omnifuzz {

void DebugFeedback::WriteOnBasicBlock(std::string& assembly) const {
  std::stringstream ss;
  ss << "push %rax\n"
     << "add $$0x1337, %rax\n"
     << "pop %rax\n";
  assembly += ss.str();
}
  
} // namespace omnifuzz
