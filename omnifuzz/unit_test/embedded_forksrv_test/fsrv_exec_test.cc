#include <string.h>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include "omnifuzz/executor/forksrv_executor.h"
#include "omnifuzz/feedback/mechanism.h"
#include "omnifuzz/feedback/fuzz_score.h"
#include "omnifuzz/feedback/debug_mechanism.h"

const size_t kNumRequest = 5;
std::string instance = "./embedded.exe";

const char* inputs[] = {
  "hello", 
  "pingjui",
  "oregon state university", 
  "I like cold brew",
  "omnifuzz"
};


int main(int argc, char** argv) {
  
  char buf[0x200];
  memset(buf, 0, sizeof(buf));
  
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
  
  std::vector<std::string> v_argv = {instance};
  
  executor.Initialize(v_argv, fdbk);
  for (size_t i = 0; i < kNumRequest; ++i) {
    memset(buf, 0, sizeof(buf));
    strcpy(buf, inputs[i]);
    executor.Execute((unsigned char *)buf, strlen(buf));
    void* p = executor.DumpFeedbackData();
    std::cout << "[Result] " << reinterpret_cast<char *>(p) << std::endl \
              << "[Message] feedback gained, cycle Completed" << std::endl \
              << std::endl << std::endl;
  }

  return 0;
}
