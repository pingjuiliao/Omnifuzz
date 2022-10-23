#include <cstring>
#include <filesystem>
#include <iostream>

#include "omnifuzz/fuzzer.h"
#include "omnifuzz/executor/forksrv_executor.h"
#include "omnifuzz/scheduler/list_scheduler.h"
#include "omnifuzz/mutator/deterministic_mutator.h"
#include "omnifuzz/feedback/afl_mechanism.h"

class BasicGreyBoxFuzzer : public omnifuzz::Fuzzer {
 public:
  BasicGreyBoxFuzzer() {
    Configure();
  }
  virtual ~BasicGreyBoxFuzzer() {}
 protected:
  void Configure(void) override {
    executor_ = new omnifuzz::ForkServerExecutor();
    scheduler_ = new omnifuzz::ListScheduler();
    mutator_ = new omnifuzz::DeterministicMutator();
    fdbk_mech_ = new omnifuzz::AFLFeedbackMechanism();
    configured_ = true;
  }
};

int main(int argc, char** argv) {
  char* executable = strdup("./program_under_test/coldbrew.exe");
  if (!std::filesystem::exists(executable)) {
    std::cerr << "[Error] Cannot find executable" << std::endl;
    return -1;
  }
  if (std::filesystem::exists(".cur_input")) {
    std::filesystem::remove(".cur_input");
  }
  if (std::filesystem::exists("./omnifuzz_out")) {
    std::filesystem::remove_all("./omnifuzz_out");
  }

  char* ARGV[] = {executable, NULL};
  BasicGreyBoxFuzzer fuzzer;
  fuzzer.LoadSeed("./program_under_test/omnifuzz_in");
  fuzzer.Prepare(ARGV);
  fuzzer.Run();
  return 0;
}
