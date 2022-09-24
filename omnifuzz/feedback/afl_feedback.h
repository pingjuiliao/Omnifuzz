#ifndef LLVM_TRANSFORMS_OMNIFUZZ_FEEDBACK_AFL_FEEDBACK_H
#define LLVM_TRANSFORMS_OMNIFUZZ_FEEDBACK_AFL_FEEDBACK_H

#include <cstdlib>
#include <sstream>

// #include "llvm/Transforms/Omnifuzz/Feedback/feedback.h"
#include "omnifuzz/feedback/feedback.h"

namespace omnifuzz {

class AflFeedback : public Feedback {
 public:
  AflFeedback();
  virtual ~AflFeedback();
  virtual void RegisterFeedbackData(void) override;
  virtual void WriteOnBasicBlock(std::string&) override;
  virtual bool DeemInteresting(void) override;
 protected:
  const unsigned int kCoverageBitMapEntry = 64 * 1024;
};

} // namespace omnifuzz

#endif  // LLVM_TRANSFORMS_OMNIFUZZ_FEEDBACK_AFL_FEEDBACK_H
