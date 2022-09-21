
#include "executor.h"


namespace omnifuzz {
Executor::~Executor() {
  if (c_argv_) {
    char** p = c_argv_;
    while (*p) {
      free(*p);
      p++;
    } 
    free(c_argv_);
  }
}

void Executor::LoadArgument(std::vector<std::string> argv) {

  // argv 
  if (!argv.size()) {
    std::cerr << "[FATAL] Wrong Argv\n";
    c_argv_ = nullptr;
    return;
  }
  c_argv_ = (char **) malloc(sizeof(char *) * (argv.size() + 1));
  for (size_t i = 0; i < argv.size(); ++i) {
    c_argv_[i] = strdup(argv[i].c_str());
  }
  c_argv_[argv.size()] = nullptr;
}

void Executor::LoadArgument(char** argv) {
  if (!argv) 
    return;
  
  size_t num_args = 0;
  char** p = argv;
  while (*p) {
    num_args++;
    p++;
  }
  c_argv_ = (char **) malloc(sizeof(char *) * (num_args + 1));
  for (size_t i = 0; i < num_args; ++i) {
    c_argv_[i] = strdup(argv[i]);     
  }
  c_argv_[num_args] = nullptr;
}
} // namespace omnifuzz
