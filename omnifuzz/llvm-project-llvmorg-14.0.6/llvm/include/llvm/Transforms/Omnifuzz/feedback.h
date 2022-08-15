#ifndef FEEDBACK_H
#define FEEDBACK_H

namespace omnifuzz {

class Feedback {
 public:
  virtual ~Feedback() = default;
  virtual void operatesOnBasicBlock(uint32_t) = 0;
};

}

#endif  // FEEDBACK_H
