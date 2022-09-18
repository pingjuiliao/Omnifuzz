#include "interesting.h"

InterestingInt8Mutator::InterestingInt8Mutator() {

}

InterestingInt8Mutator::~InterestingInt8Mutator() {}

MutationResult InterestingInt8Mutator::Mutate(uint8_t* data, size_t len) {
  if (data + index > len) {
    return MutationResult::kCycleDone;
  }

}
