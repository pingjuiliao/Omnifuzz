#ifndef OMNIFUZZ_FUZZER_H
#define OMNIFUZZ_FUZZER_H

#include <string>

#include "omnifuzz/feedback/feedback_mechanism.h"
#include "omnifuzz/mutator/mutator.h"
#include "omnifuzz/preprocessor/instrumentator.h"
#include "omnifuzz/executor/executor.h"
#include "omnifuzz/testcase_file_manager.h"

namespace omnifuzz {

class Fuzzer {
 public:
  virtual ~Fuzzer() {}
  
  // Run instrumented program without argv.
  virtual void Run(void);

 protected:
  // Assemble fuzzer parts to determine fuzzing.
  virtual void Configure(void) = 0;

  // Fuzzer parts, most of them are must-have.
  TestcaseFileManager* test_file_manager_;
  Instrumentator* instrumentator_;
  MutatorComposite* mutator_;
  Executor* executor_;
  FeedbackMechanism* fdbk_mech_;
};

} // namespace omnifuzz

#endif  // OMNIFUZZ_FUZZER_H
