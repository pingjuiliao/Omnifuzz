#ifndef AFL_FEEDBACK_SYSTEM_FACTORY_H
#define AFL_FEEDBACK_SYSTEM_FACTORY_H
#include "feedback_system_factory.h"

class AflFeedbackSystemFactory: public FeedbackSystemFactory {
 public:
  AflFeedbackSystemFactory();
  virtual ~AflFeedbackSystemFactory();

  virtual FeedbackMethod makeFeedbackMethod();
  virtual std::vector<FuzzerComponent> makeFuzzerComponent();
};
#endif // AFL_FEEDBACK_SYSTEM_FACTORY_H
