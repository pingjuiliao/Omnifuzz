#ifndef FUZZER_MUTATOR_AFL_MUTATOR_H
#define FUZZER_MUTATOR_AFL_MUTATOR_H

#include "mutator.h"
#include "bit_flip.h"
#include "arithmetic.h"
#include "interesting.h"


namespace omnifuzz {

class AflMutator: public MutatorComposite {
 public:
  AflMutator();
};

} // namespace omnifuzz

#endif  // FUZZER_MUTATOR_AFL_MUTATOR_H
