#ifndef DEBUG_FEEDBACK_H
#define DEBUG_FEEDBACK_H

#include <cstdint>
#include <iostream>

#include "llvm/Transforms/Omnifuzz/feedback.h"


namespace omnifuzz {

class DebugFeedback : public Feedback {
 public:
  DebugFeedback() {};
  virtual ~DebugFeedback() {};
  virtual void operatesOnBasicBlock(uint32_t) override;
};

}

#endif  // DEBUG_FEEDBACK_H
