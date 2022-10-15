#ifndef OMNIFUZZ_FEEDBACK_AFL_MECHANISM_H
#define OMNIFUZZ_FEEDBACK_AFL_MECHANISM_H

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <unordered_set>

#include "omnifuzz/feedback/fuzz_score.h"
#include "omnifuzz/feedback/mechanism.h"
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
  
  // TODO: generalize this.
  //  challenge, each feedback may return differnt things...
  // Interpretation
  std::vector<uint32_t> InterpretFeedbackDataForScheduler(void);
  
 protected:
  const size_t kCoverageBitMapEntry = 64 * 1024;
  // maintain a virgin bitmap to see if there's new interesting bits comes in.
  uint8_t* curr_bitmap_;
  uint8_t* virgin_map_;
  std::unordered_set<uint32_t> seen_cov_chksm_;
};

} // namespace omnifuzz

#endif  // OMNIFUZZ_FEEDBACK_AFL_MECHANISM_H
