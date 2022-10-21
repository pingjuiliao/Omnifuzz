#ifndef OMNIFUZZ_MUTATOR_DICTIONARY_H
#define OMNIFUZZ_MUTATOR_DICTIONARY_H

#include <bits/stdc++.h>
#include <cstring>
#include <vector>

#include "omnifuzz/mutator/mutator.h"

namespace omnifuzz {

using DataSequence = std::vector<uint8_t>;

class DictionaryMutator : public Mutator {
 public:
  DictionaryMutator(std::vector<DataSequence*>);
  DictionaryMutator(std::vector<std::string>);
  virtual ~DictionaryMutator();
  virtual MutationResult Mutate(uint8_t*, size_t&) override;
 protected:
  std::vector<DataSequence*> seqs_;
  std::vector<DataSequence*>::iterator iterator_;
  uint8_t* original_;
  size_t longest_seq_size_;
  uint32_t index_start_;
};

} // namespace omnifuzz

#endif  // OMNIFUZZ_MUTATOR_DICTIONARY_H
