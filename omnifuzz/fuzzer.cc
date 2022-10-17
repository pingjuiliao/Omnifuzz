#include "omnifuzz/fuzzer.h"

namespace omnifuzz {

void Fuzzer::LoadSeed(std::string seed_dir) {
  if (!configured_) {
    std::cerr << "Ambiguous fuzzer" << std::endl;
    return;
  }
  testcase_file_manager_.BuildDirectoryTree();
  input_ready_ = testcase_file_manager_.LoadSeedTestcaseFiles(scheduler_, seed_dir);
  if (input_ready_) {
    std::cout << "[Fuzzer]: Load testcase successfully" << std::endl;
  }
}

void Fuzzer::Prepare(char** argv) {
  if (!configured_) {
    std::cerr << "Ambiguous fuzzer" << std::endl;
    return;
  }
  executable_ready_ = executor_->Initialize(argv, fdbk_mech_);
  if (executable_ready_) {
    std::cout << "[Fuzzer]: Load executable successfully" << std::endl;
  }
}

void Fuzzer::Run(void) {
  Testcase* testcase;
  void* shm_feedback;
  std::cout << "[Start Running] !" << std::endl;
  if (!executable_ready_ || !input_ready_) {
    std::cerr << "[Error] executable or input not ready" << std::endl;
    return;
  }

  while (testcase = scheduler_->Dequeue()) {
    std::cout << "[FuzzOne]\n";
    if (!testcase) {
      continue;
    }
    
    uint8_t* buf = testcase_file_manager_.LoadToBuffer(testcase);
    size_t size = testcase->size;
    while (mutator_->Mutate(buf, size) != 
           MutationResult::kCycleDone) {
      
      // Run the Program Under Test
      timer_.Start(); 
      executor_->Execute(buf, size);
      auto exec_us = timer_.Stop();
      
      shm_feedback = executor_->DumpFeedbackData();

      if (executor_->CaptureCrash() &&
          fdbk_mech_->DeemUniqueCrash(shm_feedback)) {
        testcase_file_manager_.CreateCrashReport(buf, size);
      } 
      if (fdbk_mech_->DeemInteresting(shm_feedback)) {
        Testcase new_testcase;
        new_testcase.generation = testcase->generation + 1;
        new_testcase.exec_us = exec_us;
        testcase_file_manager_.CreateTestcaseFile(new_testcase, buf, size);
        scheduler_->Enqueue(new_testcase);
      }
    } 
    testcase_file_manager_.Unload(buf);
  }
}

} // namespace omnifuzz
