#ifndef FUZZER_MUTATOR_H
#define FUZZER_MUTATOR_H

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
    kAbort, 
    kNumResults,
};

class Mutator {
 public:
  
  virtual ~Mutator() {}
  
  virtual void AddMutator(Mutator*) {}
  
  // Mutation, true on Success.
  virtual MutationResult Mutate(uint8_t*, size_t) = 0;
 protected:
  virtual uint32_t GetGranularitySize(MutationGranularity);
};

class MutatorComposite : public Mutator {
 public:
  MutatorComposite();
  virtual ~MutatorComposite();
  virtual void AddMutator(Mutator*) override;
  virtual MutationResult Mutate(uint8_t*, size_t) override;
 private:
  std::list<Mutator*> mutators_;
  std::list<Mutator*>::iterator iterator_;
};


} // namespace omnifuzz

#endif  // FUZZER_MUTATOR_H
