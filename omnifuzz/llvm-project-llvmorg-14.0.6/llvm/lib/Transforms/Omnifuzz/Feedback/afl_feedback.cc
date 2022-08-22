#include "llvm/Transforms/Omnifuzz/Feedback/afl_feedback.h"

namespace omnifuzz {

void AflFeedback::WriteOnBasicBlock(std::string& assembly) const {
  std::stringstream ss;
  size_t compile_time_random = rand() % kCoverageBitMapEntry;
  ss << "push %rcx\n"
     << "push %rdx\n"
     << "movq $$" << compile_time_random << ", %rcx\n"
     << "xorq __afl_prev_loc(%rip), %rcx\n"
     << "xorq %rcx, __afl_prev_loc(%rip)\n"
     << "shrq $$1, __afl_prev_loc(%rip)\n"
     << "popq %rdx\n"
     << "popq %rcx\n";
  assembly += ss.str();
}

} // namespace omnifuzz
