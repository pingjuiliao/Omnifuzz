#include <cstring>
#include <filesystem>
#include <iostream>

#include "omnifuzz/fuzzer.h"
#include "omnifuzz/executor/forksrv_executor.h"
#include "omnifuzz/scheduler/list_scheduler.h"
#include "omnifuzz/mutator/afl_mutator.h"
#include "omnifuzz/feedback/afl_mechanism.h"

class AflFuzz : public omnifuzz::Fuzzer {
 public:
  AflFuzz() {
    Configure();
  }
  virtual ~AflFuzz() {}
 protected:
  void Configure(void) override {
    executor_ = new omnifuzz::ForkServerExecutor();
    scheduler_ = new omnifuzz::ListScheduler();
    mutator_ = new omnifuzz::AflMutator();
    fdbk_mech_ = new omnifuzz::AflFeedbackMechanism();
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
  AflFuzz afl;
  afl.LoadSeed("./program_under_test/omnifuzz_in");
  afl.Prepare(ARGV);
  afl.Run();
  return 0;
}
