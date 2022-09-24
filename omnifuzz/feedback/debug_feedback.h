#ifndef DEBUG_FEEDBACK_H
#define DEBUG_FEEDBACK_H

#include <iostream>
#include <sstream>

#include "omnifuzz/feedback/feedback.h"


namespace omnifuzz {

class DebugFeedback : public Feedback {
 public:
  DebugFeedback();
  virtual ~DebugFeedback();
  virtual bool DeemInteresting(void) override;
  virtual void RegisterFeedbackData(void) override;
  virtual void WriteOnBasicBlock(std::string&) override;
};

}

#endif  // DEBUG_FEEDBACK_H
