
#include "mutator.h"

namespace omnifuzz {

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


MutationResult MutatorComposite::Mutate(uint8_t* data, size_t len) {

  // In initialized/finished state, return to the begining
  if (iterator_ == mutators_.end()) {
    iterator_ = mutators_.begin();
  }

  MutationResult result = (*iterator_)->Mutate(data, len);
  switch (result) {
    case MutationResult::kSuccess:
      break;
    case MutationResult::kCycleDone:
      iterator_ ++;
      if (iterator_ == mutators_.end()) {
        return MutationResult::kCycleDone;
      }
      break;
    default:
      return MutationResult::kAbort;
  }
  return MutationResult::kSuccess; 
}

} // namespace omnifuzz
