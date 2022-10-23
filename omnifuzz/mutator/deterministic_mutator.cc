
#include "omnifuzz/mutator/deterministic_mutator.h"

namespace omnifuzz {
  
DeterministicMutator::DeterministicMutator() {
  // bit flips
  AddMutator(new BitFlipMutator(MutationGranularity::kBit));
  AddMutator(new BitFlipMutator(MutationGranularity::kBits2));
  AddMutator(new BitFlipMutator(MutationGranularity::kBits4));
  AddMutator(new BitFlipMutator(MutationGranularity::kByte));
  AddMutator(new BitFlipMutator(MutationGranularity::kWord));

  // arithmetic
  AddMutator(new ArithmeticMutator(MutationGranularity::kByte));
  AddMutator(new ArithmeticMutator(MutationGranularity::kWord));
  AddMutator(new ArithmeticMutator(MutationGranularity::kDword));
  
  // interesting integral
  AddMutator(new InterestingInt8Mutator());
  AddMutator(new InterestingInt16Mutator());
  AddMutator(new InterestingInt32Mutator());
}

} // namespace omnifuzz
