#include "llvm/Transforms/Omnifuzz/Feedback/afl_feedback.h"

namespace omnifuzz {

AflFeedback::AflFeedback() {
  RegisterFeedbackData();
}

AflFeedback::~AflFeedback() {}

void AflFeedback::RegisterFeedbackData(void) {
  feedback_data_map_["__afl_prev_loc"] = FeedbackData("__afl_prev_loc", FeedbackData::Type::Int32);
  feedback_data_map_["__afl_area_ptr"] = FeedbackData("__afl_area_ptr", 
      FeedbackData::Type::Pointer);
}

void AflFeedback::WriteOnBasicBlock(std::string& assembly) {
  std::stringstream ss;
  size_t compile_time_random = rand() % kCoverageBitMapEntry;
  ss << "push %rcx\n"
     << "push %rdx\n"
     << "movq $$" << compile_time_random << ", %rcx\n"
     << "xorq " << feedback_data_map_["__afl_prev_loc"].GetName() << "(%rip), %rcx\n"
     << "xorq %rcx, " << feedback_data_map_["__afl_prev_loc"].GetName() << "(%rip)\n"
     << "shrq $$1, " << feedback_data_map_["__afl_prev_loc"].GetName() << "(%rip)\n"
     << "popq %rdx\n"
     << "popq %rcx\n";
  assembly += ss.str();
}

bool AflFeedback::DeemInteresting(void) {
  return false;
}

} // namespace omnifuzz
