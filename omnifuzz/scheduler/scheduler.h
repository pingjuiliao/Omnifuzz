#ifndef OMNIFUZZ_SCHEDULER_SCHEDULER_H
#define OMNIFUZZ_SCHEDULER_SCHEDULER_H

#include <cstdint>

#include "omnifuzz/testcase.h"
/* scheduler should aware of the feedback of exection */


namespace omnifuzz {

class Scheduler {
 public:
  virtual ~Scheduler() = default;
  virtual void Enqueue(Testcase) = 0;
  virtual Testcase* Dequeue(void) = 0;
};

} // namespace omnifuzz

#endif  // OMNIFUZZ_SCHEDULER_SCHEDULER_H
