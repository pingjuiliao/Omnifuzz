#ifndef OMNIFUZZ_FEEDBACK_SYSTEM_FEEDBACK_H
#define OMNIFUZZ_FEEDBACK_STSTEM_FEEDBACK_H
#include <cstdint>

namespace omnifuzz {

class Feedback {
 public:
  virtual ~Feedback() = default;
  virtual void operationOnBasicBlock(uint32_t) = 0;
};

} // namespace omnifuzz
#endif // OMNIFUZZ_FEEDBACK_SYSTEM_FEEDBACK_H
