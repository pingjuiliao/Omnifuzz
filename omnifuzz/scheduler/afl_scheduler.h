#ifndef OMNIFUZZ_SCHEDULER_AFL_SCHEDULER_H
#define OMNIFUZZ_SCHEDULER_AFL_SCHEDULER_H

#include "omnifuzz/testcase.h"

namespace omnifuzz {

struct AFLQueue {
  Testcase testcase;
  uint8_t favored;
  uint8_t cal_failed;
  uint8_t has_new_cov;
  uint8_t fs_redundant;
  AFLQueue* next;
  AFLQueue* next_100; // the difference between std::list,...
};

class AFLScheduler : public Schduler {
 public:
  AFLScheduler();
  virtual ~AFLScheduler();
  virtual void Enqueue(Testcase) override;
  virtual Testcase* Dequeue(void) override;
 private:
  AFLQueue* queue;
  AFLQueue* queue_cur;
  AFLQueue* queue_top;
  AFLQueue* q_prev100;
};

} // namespace omnifuzz
  
#endif  // OMNIFUZZ_SCHEDULER_AFL_SCHEDULER_H
