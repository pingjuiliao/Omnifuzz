#ifndef FUZZER_EXECUTOR_EXECUTOR_H
#define FUZZER_EXECUTOR_EXECUTOR_H

#include "fork_client.h"
struct CommonFD {
  int dev_null;
  int urandom;
};

class Executor {
 public:
  Executor();
  virtual ~Executor();
  virtual void LoadBinary();
  virtual void StartSubprocess();
};

#endif  // FUZZER_EXECUTOR_EXECUTOR_H
