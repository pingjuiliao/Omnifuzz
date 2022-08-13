#ifndef OMNIFUZZ_FEEDBACK_SYSTEM_FACTORY_H
#define OMNIFUZZ_FEEDBACK_SYSTEM_FACTORY_H
#include <iostream>
#include <vector>

// temporary
typedef int FeedbackMethod;
typedef int FuzzerComponent;

class FeedbackSystemFactoryBase {
 public:
    
  FeedbackSystemFactoryBase();
  virtual ~FeedbackSystemFactoryBase();

  virtual FeedbackMethod makeFeedbackMethod() = 0; 
  virtual std::vector<FuzzerComponent> makeFuzzerComponent() = 0;
};


#endif // OMNIFUZZ_FEEDBACK_SYSTEM_FACTORY_H
