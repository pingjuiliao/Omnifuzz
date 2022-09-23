#ifndef OMNIFUZZ_SCHEDULER_AFL_SCHEDULER_H
#define OMNIFUZZ_SCHEDULER_AFL_SCHEDULER_H

#include "../testcase.h"
namespace omnifuzz {

struct AflQueue {
  Testcase testcase;
  AflQueue* next;
  AflQueue* next_100; // the difference between std::list,...
};


class AflScheduler : public Schduler {
 public:
  AflScheduler();
  virtual ~AflScheduler();
  virtual void Enqueue(Testcase*) override;
  virtual Testcase* Dequeue() override;
 private:
  AflQueue* queue;
  AflQueue* queue_cur;
  AflQueue* queue_top;
  AflQueue* q_prev100;
};

} // namespace omnifuzz
  
#endif  // OMNIFUZZ_SCHEDULER_AFL_SCHEDULER_H
