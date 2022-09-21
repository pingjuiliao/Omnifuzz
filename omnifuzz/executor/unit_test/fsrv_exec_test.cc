
#define UNIT_TEST
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include "../forksrv_executor.h"


const size_t kNumRequest = 5;
std::string instance = "./fsrv_instance.exe";

int main(int argc, char** argv) {

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
  executor.LoadExecutable(v_argv);
  for (size_t i = 0; i < kNumRequest; ++i) {
    executor.Execute();
  }

  return 0;
}
