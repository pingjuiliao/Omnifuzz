
#include "omnifuzz/scheduler/list_scheduler.h"

namespace omnifuzz {

ListScheduler::ListScheduler() {
  max_generation_ = 0;
}

ListScheduler::~ListScheduler() {
  for (auto testcase: list_) {
    if (testcase.feedback) {
      delete testcase.feedback; 
    }
  }
  list_.clear();
}

void ListScheduler::Enqueue(Testcase testcase) {
  list_.push_back(testcase);
}

// ListScheduler keep all the testcases
Testcase* ListScheduler::Dequeue(void) {
  std::list<Testcase>::iterator it = list_.begin();
  for (it = list_.begin(); it != list_.end(); ++it) {
    if (!it->fuzzed) {
      std::cout << "[Scheduler]: scheduled one\n";
      return &(*it);
    }
  }
  return nullptr;
}

} // namespace omnifuzz
