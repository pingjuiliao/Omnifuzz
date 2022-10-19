
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

  // delete top_rated_
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
  
  CullQueue();

  if (!queue_cur_) {
    queue_cur_ = queue_;
  } else {
    queue_cur_ = queue_cur_->next;
  }


  if (pending_favored_) {
    while (!queue_cur_->favored) {
      queue_cur_ = queue_cur_->next;
    }
    pending_favored_--;
  }

  // For debug purpose, concetually, pending favored must find a 
  //  favored entry
  if (!queue_cur_) { 
    return nullptr;
  }


  std::cout << "[Scheduler]: scheduled" << queue_cur_->testcase.file_name \
            << std::endl;
  return &queue_cur_->testcase;
}


// Same algorithm as afl-fuzz.c: update_bitmap_score(struct queue_entry *q)
void AFLScheduler::UpdateBitmapScore(AFLQueue *q, 
    std::unordered_map<std::string, std::pair<void*, size_t>> *fuzz_state) {
  
  // The feedback mechanism interpret the coverage bitmap as the 
  // indices that the bits are on.
  // (This may be change since I am trying to generalize the interpretation.)
  
  auto afl_bitmap_metadata = (*fuzz_state)["afl_bitmap"];
  uint8_t* afl_bitmap_ptr = static_cast<uint8_t*>(afl_bitmap_metadata.first);
  size_t afl_bitmap_size = afl_bitmap_metadata.second;

  
  if (!top_rated_) {
    top_rated_ = new AFLQueue*[afl_bitmap_size];
    memset(top_rated_, 0, sizeof(AFLQueue*) * afl_bitmap_size);
    bitmap_size_ = afl_bitmap_size;
  }

  uint64_t fav_factor = q->testcase.exec_us * q->testcase.size;
  for (int i = 0; i < afl_bitmap_size; ++i) {
    if (top_rated_[i]) {
      AFLQueue* curr_top = top_rated_[i];
      if (fav_factor > curr_top->testcase.exec_us * curr_top->testcase.size) {
        continue;
      } 
      /* 
      if (!--top_rated_[i]->tc_ref) {
        
      }*/
    }
    
    /* Insert as the new Winner */
    top_rated_[i] = q;
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
  
  if (!top_rated_) {
    return;
  }

  // Select top_rated as favored. 
  for (int i = 0; i < bitmap_size_; ++i) {
    if (top_rated_[i] /* && (temp_v[i >> 3] * (1 << (i & 7)))*/ ) {
      top_rated_[i]->favored = 1;
      queue_favored_ ++;
      if (!top_rated_[i]->testcase.was_fuzzed) {
        pending_favored_++;     
      }
    }
  }
  
  // TODO: mark as redundant
}

} // namespace omnifuzz
