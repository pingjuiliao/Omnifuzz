
#include "omnifuzz/mutator/mutator.h"

namespace omnifuzz {


// Some mutators need to make sure the mutation is of 2 to the n
uint32_t Mutator::GetGranularitySize(MutationGranularity g) {
  
  switch(g) {
    case MutationGranularity::kBit:
      return 1;
    case MutationGranularity::kBits2:
      return 2;
    case MutationGranularity::kBits4:
      return 4;
    case MutationGranularity::kByte:
      return 8;
    case MutationGranularity::kWord:
      return 16;
    case MutationGranularity::kDword:
      return 32;
    case MutationGranularity::kQword:
      return 64;
    default:
      break;  
  }
  return 0;
}


MutatorComposite::MutatorComposite() {
  iterator_ = mutators_.end();
}

MutatorComposite::~MutatorComposite() {
  for (auto mutator: mutators_) {
    delete mutator;
  }
  mutators_.clear();
}

void MutatorComposite::AddMutator(Mutator* mutator) {
  if (mutator) {
    mutators_.push_back(mutator);
  }
}


MutationResult MutatorComposite::Mutate(uint8_t* data, size_t& size, 
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

    MutationResult result = (*iterator_)->Mutate(data, size);
    if (result == MutationResult::kSuccess) {
      break;
    } else if (result == MutationResult::kCycleDone) {
      iterator_ ++;
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

MutationResult MutatorComposite::RandomMutate(uint8_t* data, size_t &size) {
  return MutationResult::kAbort;
}

} // namespace omnifuzz
