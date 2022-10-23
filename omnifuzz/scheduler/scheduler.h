#ifndef OMNIFUZZ_SCHEDULER_SCHEDULER_H
#define OMNIFUZZ_SCHEDULER_SCHEDULER_H

#include <cstdint>
#include <unordered_map>
#include <utility>
#include "omnifuzz/testcase.h"
#include "omnifuzz/fuzzer_state.h"

/* scheduler should aware of the feedback of exection */
namespace omnifuzz {

class Scheduler {
 public:
  virtual ~Scheduler() = default;
  virtual void Enqueue(Testcase, FuzzerState* = nullptr) = 0; 
  virtual Testcase* Dequeue(void) = 0;
};

} // namespace omnifuzz

#endif  // OMNIFUZZ_SCHEDULER_SCHEDULER_H
