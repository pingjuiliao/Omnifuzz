#ifndef OMNIFUZZ_MUTATOR_ARITHMETIC_H
#define OMNIFUZZ_MUTATOR_ARITHMETIC_H

#include <iostream>

#include "omnifuzz/mutator/mutator.h"

namespace omnifuzz {

class ArithmeticMutator : public Mutator {
 public:
  ArithmeticMutator(MutationGranularity);
  virtual ~ArithmeticMutator();
  virtual MutationResult Mutate(uint8_t*, size_t) override;
 protected:
  template <typename T>
  MutationResult MutateAndIterate(T*, size_t);
  const size_t kArithmeticMax = 10;
  uint8_t *ptr_start_;
  uint8_t *ptr_end_;
  size_t num_bytes_; 
  const char* name_;
};

} // namespace omnifuzz

#endif  // OMNIFUZZ_MUTATOR_ARITHMETIC_H
