#ifndef FUZZER_MUTATOR_INTERESTING_H
#define FUZZER_MUTATOR_INTERESTING_H
#include "mutator.h"

#include <vector>

namespace omnifuzz {

class InterestingInt8Mutator: public Mutator {
 public:
  InterestingInt8Mutator();
  virtual ~InterestingInt8Mutator();
  virtual MutationResult Mutate(uint8_t*, size_t) override;
 protected:
  size_t index_;
  std::vector<int8_t> values_;
};

} // namespace omnifuzz

#endif  // FUZZER_MUTATOR_INTERESTING_H
