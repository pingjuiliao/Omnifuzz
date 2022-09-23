#ifndef OMNIFUZZ_EXECUTOR_FORKSRV_EXECUTOR_H_H
#define OMNIFUZZ_EXECUTOR_FORKSRV_EXECUTOR_H_H

#include <iostream>
#include <string>
#include <vector>

#include "omnifuzz/executor/executor.h"
#include "omnifuzz/executor/file_descriptor.h"
#include "omnifuzz/executor/forkserver.h"
#include "omnifuzz/executor/fork_client.h"

namespace omnifuzz {

// maybe there's a better name
class ForkServerExecutor: public Executor {
 public:
  // Not sure the use case, but we allow the executable comes in later.
  ForkServerExecutor();
  ForkServerExecutor(std::vector<std::string>);
  virtual ~ForkServerExecutor();
  virtual bool LoadExecutable(std::vector<std::string>) override;
  virtual void Execute(void) override;
 private:
  Forkserver* fsrv_;
  ForkClient* fclnt_;
  CommonFD fd_;
};
} // namespace omnifuzz
#endif  // OMNIFUZZ_EXECUTOR_FORKSRV_EXECUTOR_H_H

