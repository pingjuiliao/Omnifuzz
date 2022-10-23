#include <cstring>
#include <cstdint>
#include <filesystem>
#include <iostream>

#include "omnifuzz/executor/forksrv_executor.h"
#include "omnifuzz/feedback/afl_mechanism.h"
#include "omnifuzz/mutator/afl_mutator.h"
#include "omnifuzz/scheduler/list_scheduler.h"
#include "omnifuzz/testcase_file_manager.h"
using namespace omnifuzz;

int main(int argc, char** argv) {
  
  // File checking
  if (std::filesystem::exists("./omnifuzz_out")) {
    std::filesystem::remove_all("./omnifuzz_out");
  }

  if (std::filesystem::exists("./.cur_input")) {
    std::filesystem::remove("./.cur_input");
  }

  char* executable = strdup("./a.exe");
  if (!std::filesystem::exists(executable)) {
    std::cerr << "[Error] cannot find executable " << executable << std::endl;
    return -1;
  }
  char* ARGV[] = {executable, NULL};

  // Configure  
  Mutator* mutator_ = new AFLMutator();
  Scheduler* scheduler_ = new ListScheduler();
  Executor* executor_ = new ForkServerExecutor();
  FeedbackMechanism* fdbk_mech_ = new AFLFeedbackMechanism();
  TestcaseFileManager testcase_file_manager_;

  // LoadSeed
  testcase_file_manager_.BuildDirectoryTree();
  bool result = testcase_file_manager_.LoadSeedTestcaseFiles(scheduler_, "./omnifuzz_in");
  if (!result) {
    std::cerr << "Cannot load seed input files" << std::endl;
    return -1;
  }

  // Prepare
  bool executable_ready = executor_->Initialize(ARGV, fdbk_mech_);
  if (!executable_ready) {
    std::cerr << "Cannot load executable" << std::endl;
    return -1;
  }

  // Run
  Testcase* testcase;
  void* shm_feedback;
  std::cout << "[Start Running] !" << std::endl;

  while (testcase = scheduler_->Dequeue()) {
    if (!testcase) {
      std::cerr << "Cannot Load testcase, skip this" << std::endl;
      continue;
    }
    
    uint8_t* buf = testcase_file_manager_.LoadToBuffer(testcase);
    size_t size = testcase->size;
    std::cout << "[FuzzOne] " << reinterpret_cast<char*>(buf) << std::endl; 
    // no mutation
    executor_->Execute(buf, size);
    shm_feedback = executor_->DumpFeedbackData();
    
    if (fdbk_mech_->DeemInteresting(shm_feedback)) {
      std::cout << "[Fuzzer] Testcase " << testcase->file_name \
                << " discovered a new behavior" \
                << std::endl;
    }

    testcase_file_manager_.Unload(buf);
  }
  return 0;
}

