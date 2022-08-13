#ifndef OMNIFUZZ_FUZZERS_AFL_FEEDBACK_H 
#define OMNIFUZZ_FUZZERS_AFL_FEEDBACK_H

#include "Omnifuzz/FeedbackSystem/feedback.h"

#include <cstdint>
#include <cstddef>
#include <iostream>

namespace omnifuzz {

class AFLFeedback : public Feedback {
 public:
  AFLFeedback();
  virtual ~AFLFeedback();
  virtual void operationOnBasicBlock(uint32_t);
 private:
  static const size_t BITMAP_SIZE = 64 * 1024;
  uint32_t prev_basic_block_id = 0;
  uint8_t bitmap[BITMAP_SIZE];
};

} // namespace omnifuzz

#endif  // OMNIFUZZ_AFL_FEEDBACK_H
