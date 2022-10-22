
#include "omnifuzz/mutator/arithmetic.h"
#include "omnifuzz/mutator/mutator.h"
#include "omnifuzz/mutator/bit_flip.h"
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
  srand(time(NULL));
}

HavocMutator::~HavocMutator() {
  
}

MutationResult HavocMutator::Mutate(uint8_t* data, size_t& size) {
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
  // TODO
  return MutationResult::kAbort;
}

} // namespace omnifuzz
