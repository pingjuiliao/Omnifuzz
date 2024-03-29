#ifndef OMNIFUZZ_SCHEDULER_LIST_SCHEDULER_H
#define OMNIFUZZ_SCHEDULER_LIST_SCHEDULER_H

#include <cstdint>
#include <iostream>
#include <list>

#include "omnifuzz/fuzzer_state.h"
#include "omnifuzz/scheduler/scheduler.h"
#include "omnifuzz/testcase.h"

namespace omnifuzz {

class ListScheduler : public Scheduler {
 public:
  ListScheduler();
  virtual ~ListScheduler();
  virtual void Enqueue(Testcase, FuzzerState* = nullptr) override;
  virtual Testcase* Dequeue(void) override;
 private:
  std::list<Testcase> list_;
  uint64_t max_generation_;
};

} // namespace omnifuzz

#endif  // OMNIFUZZ_SCHEDULER_LIST_SCHEDULER_H
