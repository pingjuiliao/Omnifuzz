
#include "omnifuzz/feedback/mechanism.h"

namespace omnifuzz {

void FeedbackMechanismComposite::AddMechanism(FeedbackMechanism* fdbk_mech) {
  mechanisms_.push_back(fdbk_mech);
}

void FeedbackMechanismComposite::RegisterExecutionVariable(void) {
  for (auto fdbk_mech: mechanisms_) {
    fdbk_mech->RegisterExecutionVariable();
  }
}

size_t FeedbackMechanismComposite::RegisterFeedbackData(void) {
  size_t total_size = 0;
  for (auto fdbk_mech: mechanisms_) {
    total_size += fdbk_mech->RegisterFeedbackData();
    fdbk_data_map_.merge(fdbk_mech->fdbk_data_map_);
  }
  return total_size;
}

void FeedbackMechanismComposite::ResetFeedbackDataState(void* ptr) {
  // BUGGY
  for (auto fdbk_mech: mechanisms_) {
    fdbk_mech->ResetFeedbackDataState(ptr);
    fdbk_data_map_.merge(fdbk_mech->fdbk_data_map_);
  }
}


void FeedbackMechanismComposite::WriteOnBasicBlock(std::string& assembly) {
  for (auto fdbk_mech: mechanisms_) {
    fdbk_mech->WriteOnBasicBlock(assembly);
  }
}



}
