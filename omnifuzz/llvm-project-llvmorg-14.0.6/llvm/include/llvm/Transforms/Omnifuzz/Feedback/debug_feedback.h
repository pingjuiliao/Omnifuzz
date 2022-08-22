#ifndef DEBUG_FEEDBACK_H
#define DEBUG_FEEDBACK_H

#include <iostream>
#include <sstream>

#include "llvm/Transforms/Omnifuzz/Feedback/feedback.h"


namespace omnifuzz {

class DebugFeedback : public Feedback {
 public:
  DebugFeedback() {};
  virtual ~DebugFeedback() {};
  virtual void WriteOnBasicBlock(std::string&) const override;
};

}

#endif  // DEBUG_FEEDBACK_H
