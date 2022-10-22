#include "omnifuzz/mutator/bit_flip.h"

namespace omnifuzz {

BitFlipMutator::BitFlipMutator(MutationGranularity g) {
  bit_index_start_ = 0;
  num_bits_ = GetGranularitySize(g);
  if (num_bits_ == 0) {
    throw "Cannot construct BitFlipMutator with weird num_bits_";
  }
}

BitFlipMutator::~BitFlipMutator() {}

MutationResult BitFlipMutator::Mutate(uint8_t* data, size_t& size) {
  const size_t size_in_bits = size << 3;

  // if the mutator has reach the very last step of its cycle
  if (bit_index_start_ + num_bits_ > size_in_bits) {
    for (int i = 0; i < num_bits_; ++i) {
      BitFlip(data, size_in_bits-1-i);
    }
    bit_index_start_ = 0;
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

MutationResult BitFlipMutator::RandomMutate(uint8_t* data, size_t& size) {
  static bool recover_mode = false;
  static uint32_t bit_position = 0;

  MutationResult result = MutationResult::kCycleDone; 

  if (!recover_mode) {
    bit_position = rand() % ((size << 3) - num_bits_ + 1);
    result = MutationResult::kSuccess;
  }
  recover_mode = !recover_mode;

  // Perform the bitflip to mutate/recover the buffer
  for (auto i = bit_position; i < bit_position + num_bits_; ++i) {
    BitFlip(data, i);
  }
  return result;
}

inline void BitFlipMutator::BitFlip(uint8_t* buf, size_t index) {
  buf[index >> 3] ^= (128 >> (index & 7));
}

} // namespace omnifuzz
