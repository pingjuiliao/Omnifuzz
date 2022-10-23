
#include "omnifuzz/mutator/havoc_mutator.h"

namespace omnifuzz {

HavocMutator::HavocMutator() {
  AddMutator(new BitFlipMutator(MutationGranularity::kBit));
  AddMutator(new BitFlipMutator(MutationGranularity::kBits2));
  AddMutator(new BitFlipMutator(MutationGranularity::kBits4));
  AddMutator(new BitFlipMutator(MutationGranularity::kByte));
  AddMutator(new BitFlipMutator(MutationGranularity::kWord));
  AddMutator(new ArithmeticMutator(MutationGranularity::kByte));
  AddMutator(new ArithmeticMutator(MutationGranularity::kWord));
  AddMutator(new ArithmeticMutator(MutationGranularity::kDword));
  AddMutator(new InterestingInt8Mutator());
  AddMutator(new InterestingInt16Mutator());
  AddMutator(new InterestingInt32Mutator());

  iterator_ = mutators_.end();
  srand(time(NULL));
}

HavocMutator::~HavocMutator() {
  
}

MutationResult HavocMutator::Mutate(uint8_t* data, size_t& size, 
                                    Testcase* testcase,
                                    FuzzerState* fuzz_state) {
  if (!data || !size) {
    return MutationResult::kInvalidBuffer;
  }
  while (true) {
    // new buffer/testcase comes in
    if (iterator_ == mutators_.end()) {
      iterator_ = mutators_.begin();
    }
    MutationResult result = (*iterator_)->RandomMutate(data, size);
    if (result == MutationResult::kSuccess) {
      break;
    } else if (result == MutationResult::kCycleDone) {
      iterator_++;
      if (iterator_ == mutators_.end()) {
        return MutationResult::kCycleDone;
      } else {
        continue;
      }
    } else {
      return MutationResult::kAbort;
    }
  }
  return MutationResult::kSuccess;
}

MutationResult HavocMutator::RandomMutate(uint8_t* data, size_t& size) {
  std::cerr << "HavocMutator does not have random mutation methods."\
            << std::endl;
  return MutationResult::kAbort; 
}

} // namespace omnifuzz
