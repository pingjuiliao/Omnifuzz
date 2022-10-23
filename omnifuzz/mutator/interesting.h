#ifndef OMNIFUZZ_MUTATOR_INTERESTING_H
#define OMNIFUZZ_MUTATOR_INTERESTING_H

#include <vector>

#include "omnifuzz/mutator/mutator.h"

namespace omnifuzz {

// Flyweight
// TODO: avoid duplicate
// TODO: Support big endian
class InterestingInt {
 public:
  InterestingInt(int64_t n): number_(n) {}
  virtual ~InterestingInt() = default;
  template <typename T>
  T GetLittleEndian(void);
 private:
  int64_t number_;
};


class InterestingInt8Mutator: public Mutator {
 public:
  InterestingInt8Mutator();
  virtual ~InterestingInt8Mutator();
  virtual MutationResult Mutate(uint8_t*, size_t&) override;
  virtual MutationResult RandomMutate(uint8_t*, size_t&) override;
 protected:
  std::vector<InterestingInt*> interesting_values_;
  std::vector<InterestingInt*>::iterator iterator_;
  uint8_t* ptr_start_;
  uint8_t* ptr_end_;
 private:
  uint8_t original_;
};

class InterestingInt16Mutator: public InterestingInt8Mutator {
 public:
  InterestingInt16Mutator();
  virtual MutationResult Mutate(uint8_t*, size_t&) override;
  virtual MutationResult RandomMutate(uint8_t*, size_t&) override;
 private:
  const size_t kNumBytes = 2;
  uint16_t original_;
}; 


class InterestingInt32Mutator: public InterestingInt16Mutator {
 public:
  InterestingInt32Mutator();
  virtual MutationResult Mutate(uint8_t*, size_t& ) override;
  virtual MutationResult RandomMutate(uint8_t*, size_t&) override;
 private:
  const size_t kNumBytes = 4;
  uint32_t original_;
}; 
} // namespace omnifuzz

#endif  // OMNIFUZZ_MUTATOR_INTERESTING_H
