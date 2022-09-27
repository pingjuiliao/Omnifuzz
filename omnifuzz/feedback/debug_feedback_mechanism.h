#ifndef OMNIFUZZ_FEEDBACK_DEBUG_FEEDBACK_MECHANISM_H
#define OMNIFUZZ_FEEDBACK_DEBUG_FEEDBACK_MECHANISM_H

#include "omnifuzz/feedback/feedback_mechanism.h"
#include "omnifuzz/feedback/fuzz_score.h"

namespace omnifuzz {

class DebugFeedbackMechanism : public FeedbackMechanism {
 public:
  DebugFeedbackMechanism();
  virtual ~DebugFeedbackMechanism();
  virtual size_t RegisterFeedbackData(void) override;
  virtual void RegisterExecutionVariable(void) override;
  virtual FuzzScore DeemInteresting(void* p) override;
  virtual void WriteOnBasicBlock(std::string &s) override;
};

} // namespace omnifuzz

#endif  // OMNIFUZZ_FEEDBACK_DEBUG_FEEDBACK_MECHANISM_H
