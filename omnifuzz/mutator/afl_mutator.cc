#include "omnifuzz/mutator/afl_mutator.h"


namespace omnifuzz {
  
AFLMutator::AFLMutator() {
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

  iterator_ = mutators_.end();
  srand(time(NULL));
}

MutationResult AFLMutator::Mutate(uint8_t* data, size_t &size,
                                  Testcase* testcase,
                                  FuzzerState* fuzz_state) {
  if (!data || !size) {
    return MutationResult::kInvalidBuffer;
  }

  MutationResult result = MutationResult::kNumResults;
  if (!testcase->was_fuzzed) {
    result = MutatorComposite::Mutate(data, size);
  } else {
    result = RandomMutate(data, size);
  }

  if (result == MutationResult::kCycleDone) {
    testcase->was_fuzzed = 1;
  }
  return result;
}


MutationResult AFLMutator::RandomMutate(uint8_t* data, size_t &size) {
  static uint32_t iterator = 0;
  static int mutator_index = -1;
  MutationResult result = MutationResult::kNumResults;
  
  // Recover previous input
  if (iterator != 0) {
    result = mutators_[mutator_index]->RandomMutate(data, size);
    if (result != MutationResult::kCycleDone) {
      throw "Mutation cannot be recovered";
    }
  }

  // if reaches the end of the cycle.
  iterator ++;
  if (iterator == kRandomStageMax) {
    iterator = 0;
    mutator_index = -1;
    return MutationResult::kCycleDone;
  }

  // Perform mutation
  mutator_index = rand() % mutators_.size();
  return mutators_[mutator_index]->RandomMutate(data, size);
}

} // namespace omnifuzz
