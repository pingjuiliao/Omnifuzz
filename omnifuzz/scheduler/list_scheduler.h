#ifndef OMNIFUZZ_SCHEDULER_LIST_SCHEDULER_H
#define OMNIFUZZ_SCHEDULER_LIST_SCHEDULER_H

#include <list>

#include "omnifuzz/scheduler/scheduler.h"
#include "omnifuzz/testcase.h"

namespace omnifuzz {

class ListScheduler : public Scheduler {
 public:
  ListScheduler();
  virtual ~ListScheduler();
  virtual void Append(Testcase) override;
  virtual Testcase* Top(void) override;
 private:
  std::list<Testcase> list_;
};

} // namespace omnifuzz

#endif  // OMNIFUZZ_SCHEDULER_LIST_SCHEDULER_H
