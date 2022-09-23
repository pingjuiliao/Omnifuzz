#ifndef OMNIFUZZ_SCHEDULER_SCHEDULER_H
#define OMNIFUZZ_SCHEDULER_SCHEDULER_H

#include <cstdint>

/* scheduler should aware of the feedback of exection */


namespace omnifuzz {

class Scheduler {
 public:
  virtual ~Scheduler();
  virtual void Enqueue(Testcase);
  virtual Testcase* Dequeue(void);
};

} // namespace omnifuzz

#endif  // OMNIFUZZ_SCHEDULER_SCHEDULER_H
