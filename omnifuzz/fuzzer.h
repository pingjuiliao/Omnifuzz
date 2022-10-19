#ifndef OMNIFUZZ_FUZZER_H
#define OMNIFUZZ_FUZZER_H

#include <iostream>
#include <string>
#include <unordered_map>

#include "omnifuzz/feedback/mechanism.h"
#include "omnifuzz/mutator/mutator.h"
#include "omnifuzz/preprocessor/instrumentator.h"
#include "omnifuzz/executor/executor.h"
#include "omnifuzz/testcase_file_manager.h"
#include "omnifuzz/timer.h"

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
  
  // Current
  std::unordered_map<std::string, std::pair<void*,size_t>> curr_fuzz_state_;

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
  Timer timer_;
  bool configured_;
  bool input_ready_;
  bool executable_ready_;
};

} // namespace omnifuzz

#endif  // OMNIFUZZ_FUZZER_H
