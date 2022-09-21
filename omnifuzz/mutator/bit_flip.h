#ifndef FUZZER_BASIC_MUTATOR_H
#define FUZZER_BASIC_MUTATOR_H

#include <exception>
#include <iostream>
#include <cstdint>

#include "mutator.h"

namespace omnifuzz {

class BitFlipMutator : public Mutator {
 public:
  BitFlipMutator(MutationGranularity);
  virtual ~BitFlipMutator();
  virtual MutationResult Mutate(uint8_t*, size_t) override;
 protected:
  inline void BitFlip(uint8_t*, size_t);
 private:
  // current flipping index. 
  uint32_t bit_index_start_; 
  // number of bits for each mutation, determined by MutationGranularity.
  uint32_t num_bits_;
};

} // namespace omnifuzz

#endif  // FUZZER_BASIC_MUTATOR_H
