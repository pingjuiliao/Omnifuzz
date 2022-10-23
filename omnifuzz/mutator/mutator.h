#ifndef OMNIFUZZ_MUTATOR_MUTATOR_H
#define OMNIFUZZ_MUTATOR_MUTATOR_H

#include <iostream>
#include <vector>

#include "omnifuzz/fuzzer_state.h"
#include "omnifuzz/testcase.h"

namespace omnifuzz {

enum class MutationGranularity {
  kBit,
  kBits2, 
  kBits4, 
  kByte, 
  kWord,
  kDword, 
  kQword,
};

enum class MutationResult {
    kSuccess=0, 
    kCycleDone,
    kInvalidBuffer, 
    kAbort, 
    kNumResults,
};

class Mutator {
 public:
  
  virtual ~Mutator() {}
  
  virtual void AddMutator(Mutator*) {}
  
  // Iterative Mutation
  virtual MutationResult Mutate(uint8_t*, size_t&, 
                                Testcase* = nullptr, 
                                FuzzerState* = nullptr) = 0;
  
  // Randomized Mutation, we let the random mutator make the random 
  // choise since each mutator knows its search space.
  virtual MutationResult RandomMutate(uint8_t*, size_t&) = 0;
 protected:
  virtual uint32_t GetGranularitySize(MutationGranularity);
};

class MutatorComposite : public Mutator {
 public:
  MutatorComposite();
  virtual ~MutatorComposite();
  virtual void AddMutator(Mutator*) override;
  virtual MutationResult Mutate(uint8_t*, size_t&,
                                Testcase* = nullptr,
                                FuzzerState* = nullptr) override;
  virtual MutationResult RandomMutate(uint8_t*, size_t&) override;
 protected:
  std::vector<Mutator*> mutators_;
  std::vector<Mutator*>::iterator iterator_;
};


} // namespace omnifuzz

#endif  // OMNIFUZZ_MUTATOR_MUTATOR_H
