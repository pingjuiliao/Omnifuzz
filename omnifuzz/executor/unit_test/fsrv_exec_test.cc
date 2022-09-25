#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include "omnifuzz/executor/forksrv_executor.h"
#include "omnifuzz/feedback/feedback_mechanism.h"
#include "omnifuzz/feedback/fuzz_score.h"

const size_t kNumRequest = 50;
std::string instance = "./fsrv_instance.exe";

class DummyFeedbackMechanism : public omnifuzz::FeedbackMechanism {
 public:
  DummyFeedbackMechanism() {}
  virtual ~DummyFeedbackMechanism() {}
  virtual size_t RegisterFeedbackData(void) override {
    return 20; 
  }
  virtual void RegisterExecutionVariable(void) override {}
  virtual omnifuzz::FuzzScore DeemInteresting(void* p) override {
    return omnifuzz::FuzzScore::kRevisitCoverage;
  }
  virtual void WriteOnBasicBlock(std::string &s) override {}
};


int main(int argc, char** argv) {
  DummyFeedbackMechanism* fdbk = new DummyFeedbackMechanism;
  omnifuzz::ForkServerExecutor executor;
  if (std::filesystem::exists("./.cur_input")) {
    std::filesystem::remove("./.cur_input");
  }
  
  std::cout << "Hello, there" << std::endl;
  if (!std::filesystem::exists(instance)) {
    std::cerr << "[ERROR] cannot find the executable" << instance << std::endl;
    return -1;
  }
  std::vector<std::string> v_argv = {instance};
  executor.Initialize(v_argv, fdbk);
  for (size_t i = 0; i < kNumRequest; ++i) {
    executor.Execute();
    void* p = executor.DumpFeedbackData();
    std::cout << "[Result] " << reinterpret_cast<char *>(p) << std::endl \
              << "[Message] feedback gained, cycle Completed" << std::endl \
              << std::endl << std::endl;
  }

  return 0;
}
