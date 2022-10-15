#ifndef OMNIFUZZ_SCHEDULER_AFL_SCHEDULER_H
#define OMNIFUZZ_SCHEDULER_AFL_SCHEDULER_H

#include "omnifuzz/testcase.h"
#include "omnifuzz/feedback/mechanism.h"

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
  AFLScheduler(FeedbackMechanism*);
  virtual ~AFLScheduler();
  virtual void Enqueue(Testcase) override;
  virtual Testcase* Dequeue(void) override;
 protected:
  void UpdateBitmapScore(AFLQueue *q);
  void CullQueue(void);
  AFLQueue* queue_;
  AFLQueue* queue_cur_;
  AFLQueue* queue_top_;
  AFLQueue* q_prev100_;
  uint32_t pending_favored_;
  uint32_t queue_favored_;
  bool score_changed_;
};

} // namespace omnifuzz
  
#endif  // OMNIFUZZ_SCHEDULER_AFL_SCHEDULER_H
