#ifndef LLVM_TRANSFORMS_OMNIFUZZ_FEEDBACK_AFL_FEEDBACK_H
#define LLVM_TRANSFORMS_OMNIFUZZ_FEEDBACK_AFL_FEEDBACK_H

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

#include "omnifuzz/feedback/fuzz_score.h"
#include "omnifuzz/feedback/feedback_mechanism.h"
#include "omnifuzz/testcase.h"

namespace omnifuzz {

class AflFeedbackMechanism : public FeedbackMechanism {
 public:
  AflFeedbackMechanism();
  virtual ~AflFeedbackMechanism();
  virtual void RegisterExecutionVariable(void) override;
  virtual void WriteOnBasicBlock(std::string&) override;
  
  virtual size_t RegisterFeedbackData(void) override;
  virtual void ResetFeedbackDataState(void*) override;

  virtual FuzzScore DeemInteresting(void*) override;
 protected:
  const size_t kCoverageBitMapEntry = 64 * 1024;
  Testcase* top_rated_;
  // maintain a virgin bitmap to see if there's new interesting bits comes in.
  uint8_t* virgin_map_; 
};

} // namespace omnifuzz

#endif  // LLVM_TRANSFORMS_OMNIFUZZ_FEEDBACK_AFL_FEEDBACK_H
