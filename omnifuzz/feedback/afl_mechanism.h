#ifndef OMNIFUZZ_FEEDBACK_AFL_MECHANISM_H
#define OMNIFUZZ_FEEDBACK_AFL_MECHANISM_H

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

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
  virtual bool DeemUniqueCrash(void*) override; 
  virtual void InterpretFeedback(void*,
      std::unordered_map<std::string, std::pair<void*,size_t>>*) override; 

 protected:
  const size_t kCoverageBitMapEntry = 64 * 1024;
  // maintain a virgin bitmap to see if there's new interesting bits comes in.
  uint8_t* curr_bitmap_;
  uint8_t* virgin_path_map_;
  uint8_t* virgin_crash_map_;
  FuzzScore HasNewBits(void*, uint8_t*);
};

} // namespace omnifuzz

#endif  // OMNIFUZZ_FEEDBACK_AFL_MECHANISM_H
