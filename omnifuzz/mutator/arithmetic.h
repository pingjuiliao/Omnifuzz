#ifndef OMNIFUZZ_MUTATOR_ARITHMETIC_H
#define OMNIFUZZ_MUTATOR_ARITHMETIC_H

#include <cstdlib>
#include <iostream>


#include "omnifuzz/mutator/mutator.h"

namespace omnifuzz {

class ArithmeticMutator : public Mutator {
 public:
  ArithmeticMutator(MutationGranularity);
  virtual ~ArithmeticMutator();
  virtual MutationResult Mutate(uint8_t*, size_t&,
                                Testcase* = nullptr,
                                FuzzerState* = nullptr) override;
  virtual MutationResult RandomMutate(uint8_t*, size_t&) override; 
 protected:
  template <typename T>
  MutationResult MutateAndIterate(T*, size_t);

  template <typename T>
  void MutateOne(T*, bool);

  const size_t kArithmeticMax = 35;
  uint8_t *ptr_start_;
  uint8_t *ptr_end_;
  size_t num_bytes_; 
  const char* name_;
};

} // namespace omnifuzz

#endif  // OMNIFUZZ_MUTATOR_ARITHMETIC_H
