#ifndef DEBUG_FEEDBACK_H
#define DEBUG_FEEDBACK_H

#include <iostream>
#include <sstream>

#include "omnifuzz/feedback/feedback_mechanism.h"


namespace omnifuzz {

class DebugFeedbackMechanism : public FeedbackMechanism {
 public:
  DebugFeedbackMechanism();
  virtual ~DebugFeedbackMechanism();
  virtual bool DeemInteresting(void) override;
  virtual void RegisterFeedbackData(void) override;
  virtual void WriteOnBasicBlock(std::string&) override;
};

}

#endif  // DEBUG_FEEDBACK_H
