#ifndef OMNIFUZZ_FEEDBACK_DEBUG_MECHANISM_H
#define OMNIFUZZ_FEEDBACK_DEBUG_MECHANISM_H

#include <string>
#include <sstream>

#include "omnifuzz/feedback/mechanism.h"
#include "omnifuzz/feedback/fuzz_score.h"
#include "omnifuzz/fuzzer_state.h"

namespace omnifuzz {

class DebugFeedbackMechanism : public FeedbackMechanism {
 public:
  DebugFeedbackMechanism();
  virtual ~DebugFeedbackMechanism();
  virtual size_t RegisterFeedbackData(void) override;
  virtual void ResetFeedbackDataState(void*) override;
  virtual void RegisterExecutionVariable(void) override;
  virtual void WriteOnBasicBlock(std::string&) override;
  virtual FuzzScore DeemInteresting(void*) override;
  virtual bool DeemUniqueCrash(void*) override;
  virtual void InterpretFeedback(void*, FuzzerState*) override; 
};

} // namespace omnifuzz

#endif  // OMNIFUZZ_FEEDBACK_DEBUG_MECHANISM_H
