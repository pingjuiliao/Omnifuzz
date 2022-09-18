#include "bit_flip.h"

namespace omnifuzz {

BitFlipMutator::BitFlipMutator(MutationGranularity g) {
  bit_index_start_ = 0;
  num_bits_ = GetGranularitySize(g);
  if (num_bits_ == 0) {
    throw "Cannot construct BitFlipMutator with weird num_bits_";
  }
}

BitFlipMutator::~BitFlipMutator() {}

MutationResult BitFlipMutator::Mutate(uint8_t* data, size_t len) {
  const size_t len_in_bits = len << 3;

  // if the mutator has reach the very last step of its cycle
  if (bit_index_start_ + num_bits_ > len_in_bits) {
    for (int i = 0; i < num_bits_; ++i) {
      BitFlip(data, len_in_bits-1-i);
    }
    return MutationResult::kCycleDone;
  }

  // Mutate 
  if (bit_index_start_ == 0) {
    for (auto i = bit_index_start_; i < bit_index_start_ + num_bits_; ++ i) {
       BitFlip(data, i);
    }
  } else {
    BitFlip(data, bit_index_start_ - 1);
    BitFlip(data, bit_index_start_ + num_bits_ - 1);
  }
  
  // Iterate
  bit_index_start_ ++;

  return MutationResult::kSuccess;
}

inline void BitFlipMutator::BitFlip(uint8_t* buf, size_t index) {
  buf[index >> 3] ^= (128 >> (index & 7));
}

} // namespace omnifuzz
