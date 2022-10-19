
#include "omnifuzz/scheduler/afl_scheduler.h"

namespace omnifuzz {

AFLScheduler::AFLScheduler() {
  pending_favored_ = 0;
  score_changed_ = false;
  pending_reschedule_ = false;
}

AFLScheduler::~AFLScheduler() {

}

void AFLScheduler::Enqueue(Testcase testcase) {
  
  AFLQueue* q = new AFLQueue;
  q->testcase = testcase;

  if (queue_top_) {
    queue_top_->next = q;
    queue_top_ = q;
  } else {
    q_prev100_ = queue_ = queue_top_ = q;
  }
  
  /*queued_paths_++;
  pending_not_fuzzed_++;
  
  cycles_wo_finds_ = 0;*/
  /*
  if ((queued_paths - 1) % 100 == 0 && queued_paths > 1) {
    q_prev100_->next_100 = q;
    q_prev100_ = q;
  }*/

  pending_reschedule_ = true;
}

Testcase* AFLScheduler::Dequeue(void) {
  
  CullQueue();
  
  if (!queue_cur_) {
    queue_cur_ = queue_;
  } else {
    queue_cur_ = queue_cur_->next;
  }


  if (!pending_favored_) {
    return queue_cur_;
  }

  while (!queue_cur_->favored) {
    queue_cur_ = queue_cur_->next;
  }
  pending_favored_--;
  
  return &queue_cur_->testcase;
}


// Same algorithm as afl-fuzz.c: update_bitmap_score(struct queue_entry *q)
void AFLScheduler::UpdateBitmapScore(AFLQueue *q, 
    std::unordered_map<std::string, std::pair<void*, size_t>> &fuzz_state) {
  
  // The feedback mechanism interpret the coverage bitmap as the 
  // indices that the bits are on.
  // (This may be change since I am trying to generalize the interpretation.)
  //
  void* afl_bitmap_metadata = fuzz_state["afl_bitmap"];
  void* afl_bitmap_ptr = static_cast<uint8_t>(afl_bitmap_metadata.first);
  size_t afl_bitmap_size = afl_bitmap_metadata.second;

  uint64_t fav_factor = q->testcase.exec_us * q->testcase.size;
  for (int i = 0; i < afl_bitmap_size; ++i) {
    if (top_rated_[index]) {
      AFLQueue* curr_top = top_rated_[index];
      if (fav_factor > curr_top->testcase.exec_us * curr_top->testcase.size) {
        continue;
      } 
      /* 
      if (!--top_rated_[i]->tc_ref) {
        
      }*/
    }
    
    /* Insert as the new Winner */
    top_rated_[index] = q;
    // q->tc_ref++;
    
    score_changed_ = true;
  }
}


void AFLScheduler::CullQueue(void) {
  
  // Initialize
  score_changed_ = false;
  queue_favored_ = 0;
  pending_favored_ = 0;
  
  // Mark all as unfavored
  AFLQueue* q = queue_;
  while (q) {
    q->favored = 0;
    q = q->next;
  }
  
  // Select top_rated as favored. 
  for (int i = 0; i < fdbk_mech_->kCoverageBitMapEntry; ++i) {
    if (top_rated_[i] /* && (temp_v[i >> 3] * (1 << (i & 7)))*/ ) {
      top_rated_[i]->favored = 1;
      queue_favored_ ++;
      if (!top_rated_[i]->testcase.was_fuzzed)
        pending_favored++;
    }
  }
  
  // TODO: mark as redundant
}

void AFLScheduler::Reschedule(std::unordered_map<std::string, std::pair<void*, size_t>> &fuzz_state) {
  if (!pending_reschedule_) {
    return;
  }
  pending_reschedule_ = false;
  UpdateBitmapScore(pending_reschedule, fuzz_state);
}

} // namespace omnifuzz
