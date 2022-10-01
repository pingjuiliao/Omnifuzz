
#include "omnifuzz/feedback/afl_feedback_mechanism.h" 

namespace omnifuzz {

AflFeedbackMechanism::AflFeedbackMechanism() {
  virgin_map_ = new uint8_t[kCoverageBitMapEntry];
  memset(virgin_map_, 255, kCoverageBitMapEntry);
  RegisterFeedbackData(); 
  RegisterExecutionVariable();
}

AflFeedbackMechanism::~AflFeedbackMechanism() {
  delete[] virgin_map_;
}

size_t AflFeedbackMechanism::RegisterFeedbackData(void) {
  fdbk_data_map_["__afl_area_ptr"] = ExecutionVariable("__afl_area_ptr", ExecutionVariable::Type::Pointer, kCoverageBitMapEntry);
  fdbk_data_map_["__afl_area_end"] = ExecutionVariable("__afl_area_end", ExecutionVariable::Type::Pointer, 0);
  return fdbk_data_map_["__afl_area_ptr"].GetSize();
}

void AflFeedbackMechanism::ResetFeedbackDataState(void* ptr) {
  void *afl_area_ptr = ptr;
  memset(afl_area_ptr, 0, kCoverageBitMapEntry);
}

void AflFeedbackMechanism::RegisterExecutionVariable(void) {
  exec_var_map_["__afl_prev_loc"] = ExecutionVariable("__afl_prev_loc", ExecutionVariable::Type::Int32);
}

void AflFeedbackMechanism::WriteOnBasicBlock(std::string& assembly) {
  std::stringstream ss;
  size_t compile_time_random = rand() % kCoverageBitMapEntry;
  ss << "push %rcx;\n"
     << "push %rdx;\n"
     << "movq " << fdbk_data_map_["__afl_area_ptr"].GetName() << "(%rip), %rdx\n"
     << "movq $$" << compile_time_random << ", %rcx;\n"
     << "xorq " << exec_var_map_["__afl_prev_loc"].GetName() << "(%rip), %rcx;\n"
     << "xorq %rcx, " << exec_var_map_["__afl_prev_loc"].GetName() << "(%rip);\n"
     << "shrq $$1, " << exec_var_map_["__afl_prev_loc"].GetName() << "(%rip);\n"
     << "incb (%rdx, %rcx, 1);\n"
     << "popq %rdx;\n"
     << "popq %rcx;\n";
  assembly += ss.str();
}

FuzzScore AflFeedbackMechanism::DeemInteresting(void* data) {
  uint64_t* current = reinterpret_cast<uint64_t*>(data);
  uint64_t* virgin = reinterpret_cast<uint64_t*>(virgin_map_);
  uint32_t i = (kCoverageBitMapEntry >> 3);
  
  FuzzScore score = FuzzScore::kNotInteresting;

  while (i--) {

    if (__builtin_expect(*current, 0) && 
        __builtin_expect(*current & *virgin, 0)) {
      
      if (__builtin_expect(score < FuzzScore::kNewVisitCoverage, 1)) {

        uint8_t* cur = reinterpret_cast<uint8_t*>(current);
        uint8_t* vir = reinterpret_cast<uint8_t*>(virgin);
        if ((cur[0] && vir[0] == 0xff) || (cur[1] && vir[1] == 0xff) ||
            (cur[2] && vir[2] == 0xff) || (cur[3] && vir[3] == 0xff) ||
            (cur[4] && vir[4] == 0xff) || (cur[5] && vir[5] == 0xff) ||
            (cur[6] && vir[6] == 0xff) || (cur[7] && vir[7] == 0xff)) {
          score = FuzzScore::kNewVisitCoverage;
        } else {
          score = FuzzScore::kRevisitCoverage;
        }
      }

      // update virgin map
      *virgin &= ~*current;
    }
    current ++;
    virgin ++;
  }
  switch (score) {
   case FuzzScore::kNotInteresting:
    std::cout << "[fdbk] NOT\n";
    break;
   default:
    std::cout << "[fdbk] Interesting\n";
    break;
  }
  return score;
}

} // namespace omnifuzz
