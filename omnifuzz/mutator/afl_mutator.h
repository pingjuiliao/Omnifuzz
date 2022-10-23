#ifndef OMNIFUZZ_MUTATOR_AFL_MUTATOR_H
#define OMNIFUZZ_MUTATOR_AFL_MUTATOR_H

#include "omnifuzz/mutator/mutator.h"
#include "omnifuzz/mutator/bit_flip.h"
#include "omnifuzz/mutator/arithmetic.h"
#include "omnifuzz/mutator/interesting.h"
#include "omnifuzz/mutator/havoc_mutator.h"

namespace omnifuzz {

class AFLMutator: public MutatorComposite {
 public:
  AFLMutator();
  // virtual AFLMutator::Mutate(uint8_t*, size_t&) override;
 protected:
  const int kHavocStageMax = 1024;
  HavocMutator* havoc_mutator_;
};

} // namespace omnifuzz

#endif  // OMNIFUZZ_MUTATOR_AFL_MUTATOR_H
