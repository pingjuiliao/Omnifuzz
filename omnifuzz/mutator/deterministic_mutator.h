#ifndef OMNIFUZZ_MUTATOR_DETERMINISTIC_MUTATOR_H
#define OMNIFUZZ_MUTATOR_DETERMINISTIC_MUTATOR_H

#include "omnifuzz/mutator/mutator.h"
#include "omnifuzz/mutator/bit_flip.h"
#include "omnifuzz/mutator/arithmetic.h"
#include "omnifuzz/mutator/interesting.h"

namespace omnifuzz {

class DeterministicMutator: public MutatorComposite {
 public:
  DeterministicMutator();
};

} // namespace omnifuzz

#endif  // OMNIFUZZ_MUTATOR_DETERMINISTIC_MUTATOR_H
