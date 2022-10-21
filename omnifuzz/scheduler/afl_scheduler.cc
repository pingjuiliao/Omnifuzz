
#include "omnifuzz/scheduler/afl_scheduler.h"

namespace omnifuzz {

AFLScheduler::AFLScheduler() {
  // Management of AFLQueue*
  queue_ = queue_cur_ = queue_top_ = q_prev100_ = nullptr;

  // The selection algorithm in AFL
  top_rated_ = nullptr;
  bitmap_size_ = 0;

  // global state of this scheduler
  queue_favored_ = 0;
  pending_favored_ = 0;
  score_changed_ = false;
}

AFLScheduler::~AFLScheduler() {
  // Deallocate Queue
  AFLQueue* q = queue_;
  while (q) {
    AFLQueue* tmp = q->next;
    delete q;
    q = tmp;
  }
  queue_ = queue_cur_ = queue_top_ = q_prev100_ = nullptr;

  // Deallocate top_rated_
  delete[] top_rated_;
  top_rated_ = nullptr;
}

void AFLScheduler::Enqueue(Testcase testcase, 
    std::unordered_map<std::string, std::pair<void*, size_t>> *fuzz_state) {
  
  AFLQueue* q = new AFLQueue;
  memset(q, 0, sizeof(AFLQueue));
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
  if (!fuzz_state) {
    return;
  }
  UpdateBitmapScore(q, fuzz_state);
}

Testcase* AFLScheduler::Dequeue(void) {
  
  // if we have new winner, mark them as favorate  
  if (score_changed_) {
    CullQueue();
    score_changed_ = false;
  }
  
  // Select next entry in the queue
  if (!queue_cur_) {
    queue_cur_ = queue_;
  } else {
    queue_cur_ = queue_cur_->next;
  }

  // Favorate Mode: we have a winner for a certain path, search for it
  if (pending_favored_) {
    while (!queue_cur_->favored || queue_cur_->testcase.was_fuzzed) {
      queue_cur_ = queue_cur_->next;
      if (!queue_cur_) {
        queue_cur_ = queue_;
      }
    }
    pending_favored_--;
  }

  if (!queue_cur_) {
    queue_cur_ = queue_; 
  }

  std::cout << "\n[Scheduler]: scheduled" << queue_cur_->testcase.file_name \
            << std::endl;
  std::cout << "             favored? " << ((queue_cur_->favored)? "true": "false") << std::endl;
  std::cout << "          was_fuzzed? " << ((queue_cur_->testcase.was_fuzzed)? "true": "false") << std::endl;
  queue_cur_->testcase.was_fuzzed = 1;
  return &queue_cur_->testcase;
}


// Same algorithm as afl-fuzz.c: update_bitmap_score(struct queue_entry *q)
// Select the winner for the given path, and prioritize them if they are 
//  not fuzzed.
void AFLScheduler::UpdateBitmapScore(AFLQueue *q, 
    std::unordered_map<std::string, std::pair<void*, size_t>> *fuzz_state) {
  
  // top_rated_ should be setup once 
  if (!top_rated_) {
    auto bitmap_metadata = (*fuzz_state)["afl_bitmap"];
    afl_bitmap_ptr_ = static_cast<uint8_t*>(bitmap_metadata.first);
    bitmap_size_ = bitmap_metadata.second;
    std::cout << "[Scheduler] top_rated has been set with size: " \
              << bitmap_size_ << std::endl;
    top_rated_ = new AFLQueue*[bitmap_size_];
    memset(top_rated_, 0, sizeof(AFLQueue*) * bitmap_size_);
  }

  uint64_t fav_factor = FitnessScore(q);

  for (int i = 0; i < bitmap_size_; ++i) {
    if (afl_bitmap_ptr_[i]) {
      if (top_rated_[i]) {
        // Winner: old candidate
        if (fav_factor > FitnessScore(top_rated_[i])) {
          continue;
        } 

        // Winner: new candidate, so delete the old one
        /* 
        if (!--top_rated_[i]->tc_ref) {
        
        }*/
      }
    
      /* Insert as the new Winner */
      top_rated_[i] = q;
      score_changed_ = true;
    }
  }
}


void AFLScheduler::CullQueue(void) {
  
  // Initialize
  queue_favored_ = 0;
  pending_favored_ = 0;
  
  /* TODO: I believe the algorithm of pending_favored is not complete.
   * it keeps the counts of the favorates but cancel them in CullQueue().
   * Ideally, the cancellation should not be done. Instead, the number can be
   * used to determine how many entries should be fuzzed for the following 
   * rounds/cycles.*/

  // Mark all as unfavored
  AFLQueue* q = queue_;
  while (q) {
    q->favored = 0;
    q = q->next;
  }
  
  if (!top_rated_) {
    return;
  }

  // Select top_rated as favored. 
  for (int i = 0; i < bitmap_size_; ++i) {
    if (top_rated_[i] /* && (temp_v[i >> 3] * (1 << (i & 7)))*/) {
      top_rated_[i]->favored = 1;
      queue_favored_ ++;
      if (!top_rated_[i]->testcase.was_fuzzed) {
        pending_favored_++;     
      }
    }
  }
  
  // TODO: mark redundant testcases
}

uint64_t AFLScheduler::FitnessScore(AFLQueue* q) {
  return q->testcase.size * q->testcase.exec_us;
}



} // namespace omnifuzz
