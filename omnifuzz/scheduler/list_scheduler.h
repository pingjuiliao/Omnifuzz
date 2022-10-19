#ifndef OMNIFUZZ_SCHEDULER_LIST_SCHEDULER_H
#define OMNIFUZZ_SCHEDULER_LIST_SCHEDULER_H

#include <cstdint>
#include <iostream>
#include <list>

#include "omnifuzz/scheduler/scheduler.h"
#include "omnifuzz/testcase.h"

namespace omnifuzz {

class ListScheduler : public Scheduler {
 public:
  ListScheduler();
  virtual ~ListScheduler();
  virtual void Enqueue(Testcase) override;
  virtual Testcase* Dequeue(void) override;
  virtual void Reschedule(
      std::unordered_map<std::string, std::pair<void*, size_t>>&) override;
 private:
  std::list<Testcase> list_;
  uint64_t max_generation_;
};

} // namespace omnifuzz

#endif  // OMNIFUZZ_SCHEDULER_LIST_SCHEDULER_H
