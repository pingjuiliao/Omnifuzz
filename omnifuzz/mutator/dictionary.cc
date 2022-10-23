
#include "omnifuzz/mutator/dictionary.h"

namespace omnifuzz {

bool DataSequencePtrCompare(DataSequence* d0, DataSequence* d1) {
  return d0->size() < d1->size();
}

DictionaryMutator::DictionaryMutator(std::vector<DataSequence*> seqs) {
  seqs_ = seqs;
  std::sort(seqs_.begin(), seqs_.end(), DataSequencePtrCompare);
  longest_seq_size_ = seqs_[seqs_.size()-1]->size();
  index_start_ = -1;
  original_ = nullptr;
  iterator_ = seqs_.begin();
};

DictionaryMutator::DictionaryMutator(std::vector<std::string> strings) {
  // setup sequences and its longest size
  for (auto s: strings) {
    DataSequence *seq = new DataSequence;
    for (int i = 0; i < s.size(); ++i) {
      seq->push_back(s[i]);
    }
    seqs_.push_back(seq);
  }
  std::sort(seqs_.begin(), seqs_.end(), DataSequencePtrCompare);
  longest_seq_size_ = seqs_[seqs_.size()-1]->size();
  index_start_ = -1;
  original_ = nullptr;
  iterator_ = seqs_.begin();
}

 
DictionaryMutator::~DictionaryMutator() {
  for (auto seq: seqs_) {
    seq->clear();
    delete seq;
  }
  seqs_.clear();
}

MutationResult DictionaryMutator::Mutate(uint8_t* data, size_t& size, 
                                         Testcase* testcase,
                                         FuzzerState* fuzz_state) {
  
  if (!original_) {
    index_start_ = 0;
    original_ = new uint8_t[size];
    memcpy(original_, data, size);
  }
  
  if (iterator_ == seqs_.end()) {
    memcpy(&data[index_start_], &original_[index_start_], longest_seq_size_);
    index_start_++;
    iterator_ = seqs_.begin();
  }

  if (index_start_ + longest_seq_size_ > size) {
    index_start_ = -1;
    delete[] original_;
    original_ = nullptr;
    return MutationResult::kCycleDone;
  }

  // mutate
  for (uint32_t i = 0; i < (*iterator_)->size(); ++i) {
    data[index_start_ + i] = (*iterator_)->at(i);
  }
  iterator_++;
  
  return MutationResult::kSuccess;
}

} // namespace omnifuzz
