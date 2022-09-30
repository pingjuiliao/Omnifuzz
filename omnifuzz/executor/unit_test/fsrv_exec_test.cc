#include <string.h>

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include "omnifuzz/executor/forksrv_executor.h"
#include "omnifuzz/feedback/feedback_mechanism.h"
#include "omnifuzz/feedback/fuzz_score.h"
#include "omnifuzz/feedback/debug_feedback_mechanism.h"

const size_t kNumRequest = 5;
std::string instance = "./fsrv_instance.exe";

int main(int argc, char** argv) {
  char hello[] = "hello";
  omnifuzz::DebugFeedbackMechanism* fdbk = new omnifuzz::DebugFeedbackMechanism;
  omnifuzz::ForkServerExecutor executor;
  if (std::filesystem::exists("./.cur_input")) {
    std::filesystem::remove("./.cur_input");
  }
  
  std::cout << "Hello, there" << std::endl;
  if (!std::filesystem::exists(instance)) {
    std::cerr << "[ERROR] cannot find the executable" << instance << std::endl;
    return -1;
  }
  
  std::vector<std::string> v_argv = {"./fsrv_instance.exe"};
  // char* v_argv[] = {strdup("./fsrv_instance.exe"), nullptr};
  
  executor.Initialize(v_argv, fdbk);
  for (size_t i = 0; i < kNumRequest; ++i) {
    executor.Execute((unsigned char *)hello, strlen(hello));
    void* p = executor.DumpFeedbackData();
    std::cout << "[Result] " << reinterpret_cast<char *>(p) << std::endl \
              << "[Message] feedback gained, cycle Completed" << std::endl \
              << std::endl << std::endl;
  }

  return 0;
}
