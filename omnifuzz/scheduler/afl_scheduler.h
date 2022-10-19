#ifndef OMNIFUZZ_SCHEDULER_AFL_SCHEDULER_H
#define OMNIFUZZ_SCHEDULER_AFL_SCHEDULER_H

#include <cstring>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

#include "omnifuzz/scheduler/scheduler.h"
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

class AFLScheduler : public Scheduler {
 public:
  AFLScheduler();
  virtual ~AFLScheduler();
  virtual void Enqueue(Testcase, 
      std::unordered_map<std::string, std::pair<void*, size_t>>* 
      = nullptr) override; 
  virtual Testcase* Dequeue(void) override;
 protected:
  void UpdateBitmapScore(AFLQueue *q, 
      std::unordered_map<std::string, std::pair<void*, size_t>>*);
  void CullQueue(void);
  uint64_t FitnessScore(AFLQueue*);
  AFLQueue* queue_;
  AFLQueue* queue_cur_;
  AFLQueue* queue_top_;
  AFLQueue* q_prev100_;

  // path winner: not initialized until get the size
  // i.e. AFLQueue* top_rated_[BITMAP_SIZE];
  AFLQueue** top_rated_;
  uint8_t* afl_bitmap_ptr_;
  size_t bitmap_size_;
  
  // global property
  uint32_t pending_favored_;
  uint32_t queue_favored_;
  bool score_changed_;
};

} // namespace omnifuzz
  
#endif  // OMNIFUZZ_SCHEDULER_AFL_SCHEDULER_H
