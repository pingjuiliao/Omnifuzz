#ifndef OMNIFUZZ_FEEDBACK_DEBUG_FEEDBACK_MECHANISM_H
#define OMNIFUZZ_FEEDBACK_DEBUG_FEEDBACK_MECHANISM_H

class DummyFeedbackMechanism : public omnifuzz::FeedbackMechanism {
 public:
  DummyFeedbackMechanism() {}
  virtual ~DummyFeedbackMechanism() {}
  virtual size_t RegisterFeedbackData(void) override {
    return 100; 
  }
  virtual void RegisterExecutionVariable(void) override {}
  virtual omnifuzz::FuzzScore DeemInteresting(void* p) override {
    return omnifuzz::FuzzScore::kRevisitCoverage;
  }
  virtual void WriteOnBasicBlock(std::string &s) override {
  }
};

#endif  // OMNIFUZZ_FEEDBACK_DEBUG_FEEDBACK_MECHANISM_H
