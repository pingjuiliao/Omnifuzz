#ifndef FEEDBACK_H
#define FEEDBACK_H
#include <string>

namespace omnifuzz {

class Feedback {
 public:
  virtual ~Feedback() = default;
  virtual void WriteOnBasicBlock(std::string&) const = 0;
};

}

#endif  // FEEDBACK_H
