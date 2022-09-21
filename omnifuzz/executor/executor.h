#ifndef FUZZER_EXECUTOR_EXECUTOR_H
#define FUZZER_EXECUTOR_EXECUTOR_H

#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>
#include <vector>

namespace omnifuzz {

class Executor {
 public:
  virtual ~Executor();
  // Executor needs to wait for preprocessor so we cannot always 
  // set up at initialization
  virtual bool LoadExecutable(std::vector<std::string>) = 0;
  virtual void Execute(void) = 0;
 protected:
  virtual void LoadArgument(std::vector<std::string>);
  virtual void LoadArgument(char**);
  char** c_argv_;
};
} // namespace omnifuzz

#endif  // FUZZER_EXECUTOR_EXECUTOR_H
