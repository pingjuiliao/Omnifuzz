#ifndef OMNIFUZZ_FUZZER_H
#define OMNIFUZZ_FUZZER_H

#include <string>

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
  
  // Run instrumented program without argv.
  virtual void Run(void);

 protected:
  // Assemble fuzzer parts to determine fuzzing.
  virtual void Configure(void);

  // Fuzzer parts, most of them are must-have.
  Instrumentator* instrumentator_;
  MutatorComposite* mutator_;
  Executor* executor_;
};

} // namespace omnifuzz

#endif  // OMNIFUZZ_FUZZER_H
