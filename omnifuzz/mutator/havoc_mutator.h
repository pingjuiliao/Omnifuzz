#ifndef OMNIFUZZ_MUTATOR_HAVOC_MUTATOR_H
#define OMNIFUZZ_MUTATOR_HAVOC_MUTATOR_H

#include "omnifuzz/mutator/mutator.h"
#include "omnifuzz/mutator/bit_flip.h"
#include "omnifuzz/mutator/arithmetic.h"
#include "omnifuzz/mutator/interesting.h"

namespace omnifuzz {

class HavocMutator : public MutatorComposite {
 public:
  HavocMutator();
  virtual ~HavocMutator();
  virtual MutationResult Mutate(uint8_t*, size_t&,
                                Testcase* = nullptr,
                                FuzzerState* = nullptr) override;
  virtual MutationResult RandomMutate(uint8_t*, size_t&) override;
};

} // namespace omnifuzz

#endif  // OMNIFUZZ_MUTATOR_HAVOC_MUTATOR_H
