#ifndef OMNIFUZZ_FUZZER_H
#define OMNIFUZZ_FUZZER_H

#include <string>

#include "omnifuzz/mutator/mutator.h"
#include "omnifuzz/preprocessor/instrumentor.h"
#include "omnifuzz/executor/executor.h"

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
