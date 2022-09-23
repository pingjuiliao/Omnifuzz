#include "omnifuzz/fuzzer.h"

namespace omnifuzz {

uintptr_t *shm_feedback;
void Fuzzer::CompileAndFuzzCycle(void) {
  bool r = instrumentator_.Instrument();
  while (r) {
    Run();
  }
}

void Fuzzer::Prepare(char** argv) {
  executor_->LoadExecutable(argv);
  testcase_file_manager_->BuildDirectoryTree();
}

void Fuzzer::Run(void) {
  Testcase* curr_testcase;
  char* buf = malloc(kBufSizeMax);

  while (1) {
    curr_testcase = scheduler_.Dequeue(buf);
     
    while (mutator_.Mutate(buf, testcase.size) != 
           omnifuzz::MutationResult::kCycleDone) {
      
      executor_.ExecuteWithInput(buf);
      shm_feedback = executor_.DumpFeedback();

      if (executor_->CaptureCrash()) {
        testcase_file_manager_.CreateCrashReport();
      } 
      if (feedback_.DeemInteresting(shm_feedback)) {
        Testcase new_testcase = new Testcase();
        new_testcase.generation = curr_testcase->generation + 1;
        testcase_file_manager_.CreateTestcaseFile(new_testcase);
        scheduler_.Enqueue(new_testcase);
      }
    } 
  }
}

} // namespace omnifuzz
