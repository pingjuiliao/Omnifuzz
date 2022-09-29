#include "omnifuzz/fuzzer.h"

namespace omnifuzz {

void Fuzzer::LoadSeed(std::string seed_dir) {
  if (!configured_) {
    std::cerr << "Ambiguous fuzzer" << std::endl;
    return;
  }
  testcase_file_manager_.LoadSeedTestcaseFiles(scheduler_, seed_dir);
}

void Fuzzer::Prepare(char** argv) {
  if (!configured_) {
    std::cerr << "Ambiguous fuzzer" << std::endl;
    return;
  }
  executor_->Initialize(argv, fdbk_mech_);
  testcase_file_manager_.BuildDirectoryTree();
  ready_ = true;
}

void Fuzzer::Run(void) {
  Testcase* testcase;
  void* shm_feedback;

  while (testcase = scheduler_->Dequeue()) {
    
    uint8_t* buf = testcase_file_manager_.LoadToBuffer(testcase);
    size_t size = testcase->size;
    while (mutator_->Mutate(buf, size) != 
           MutationResult::kCycleDone) {
      
      executor_->Execute(buf, size);
      shm_feedback = executor_->DumpFeedbackData();

      if (executor_->CaptureCrash()) {
        testcase_file_manager_.CreateCrashReport(buf, size);
      } 
      if (fdbk_mech_->DeemInteresting(shm_feedback)) {
        Testcase new_testcase;
        new_testcase.size = size;
        new_testcase.generation = testcase->generation + 1;
        testcase_file_manager_.CreateTestcaseFile(new_testcase, buf, size);
        scheduler_->Enqueue(new_testcase);
      }
    } 
    testcase_file_manager_.Unload(buf);
  }
}

} // namespace omnifuzz
