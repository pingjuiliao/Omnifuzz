#ifndef OMNIFUZZ_MUTATOR_MUTATOR_H
#define OMNIFUZZ_MUTATOR_MUTATOR_H

#include <iostream>
#include <list>

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
  virtual MutationResult Mutate(uint8_t*, size_t&) = 0;
  
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
  virtual MutationResult Mutate(uint8_t*, size_t&) override;
 protected:
  std::list<Mutator*> mutators_;
  std::list<Mutator*>::iterator iterator_;
};


} // namespace omnifuzz

#endif  // OMNIFUZZ_MUTATOR_MUTATOR_H
