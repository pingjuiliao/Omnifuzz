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

  // Prepare for fuzzing
  virtual void Prepare(char**);

  // Load seed inputs
  virtual void LoadSeed(std::string);

 protected:
  // Assemble fuzzer parts to determine fuzzing.
  virtual void Configure(void) = 0;

  // Fuzzer parts, most of them are must-have.
  TestcaseFileManager testcase_file_manager_;
  // Instrumentator* instrumentator_;
  Scheduler* scheduler_;
  Mutator* mutator_;
  Executor* executor_;
  FeedbackMechanism* fdbk_mech_;
  bool configured_;
  bool ready_;
};

} // namespace omnifuzz

#endif  // OMNIFUZZ_FUZZER_H
