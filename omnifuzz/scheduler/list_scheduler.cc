#include "list_scheduler.h"
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
Testcase* ListSchduler::Dequeue(void) {
  Testcase* testcase = &(list_->back());
  testcase_->is_fuzzed = true;
  testcase_->favored = false;
  return testcase;
}

} // namespace omnifuzz
