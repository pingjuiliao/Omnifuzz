#ifndef OMNIFUZZ_FUZZER_H
#define OMNIFUZZ_FUZZER_H

#define UNIT_TEST
#ifndef UNIT_TEST
#include "omnifuzz/mutator/mutator.h"
#include "omnifuzz/preprocessor/instrumentor.h"
#include "omnifuzz/executor/executor.h"
#else 
#include "mutator/mutator.h"
#include "preprocessor/instrumentator.h"
#include "executor/executor.h"
#endif

namespace omnifuzz {
class Fuzzer {
 public:
  virtual ~Fuzzer() {}
  virtual void Run(void);
 protected:
  virtual void Configure(void);
  Instrumentator* instrumentator_;
  MutatorComposite* mutator_;
  Executor* executor_;
};

} // namespace omnifuzz

#endif  // OMNIFUZZ_FUZZER_H
