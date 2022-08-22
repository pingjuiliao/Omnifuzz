#ifndef LLVM_TRANSFORMS_OMNIFUZZ_FEEDBACK_AFL_FEEDBACK_H
#define LLVM_TRANSFORMS_OMNIFUZZ_FEEDBACK_AFL_FEEDBACK_H

#include <cstdlib>
#include <sstream>

#include "llvm/Transforms/Omnifuzz/Feedback/feedback.h"

namespace omnifuzz {

class AflFeedback : public Feedback {
 public:
  AflFeedback() {}
  virtual ~AflFeedback() {}
  virtual void WriteOnBasicBlock(std::string&) const override;
 protected:
  unsigned int kCoverageBitMapEntry = 64 * 1024;
};

} // namespace omnifuzz

#endif  // LLVM_TRANSFORMS_OMNIFUZZ_FEEDBACK_AFL_FEEDBACK_H