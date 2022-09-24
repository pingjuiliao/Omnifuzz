#ifndef LLVM_TRANSFORMS_OMNIFUZZ_FEEDBACK_AFL_FEEDBACK_H
#define LLVM_TRANSFORMS_OMNIFUZZ_FEEDBACK_AFL_FEEDBACK_H

#include <cstdlib>
#include <sstream>

#include "omnifuzz/feedback/feedback_mechanism.h"

namespace omnifuzz {

class AflFeedbackMechanism : public FeedbackMechanism {
 public:
  AflFeedbackMechanism();
  virtual ~AflFeedbackMechanism();
  virtual void RegisterFeedbackData(void) override;
  virtual void WriteOnBasicBlock(std::string&) override;
  virtual bool DeemInteresting(void) override;
 protected:
  const unsigned int kCoverageBitMapEntry = 64 * 1024;
};

} // namespace omnifuzz

#endif  // LLVM_TRANSFORMS_OMNIFUZZ_FEEDBACK_AFL_FEEDBACK_H
