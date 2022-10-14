#ifndef OMNIFUZZ_EXECUTOR_FORKSRV_EXECUTOR_H_H
#define OMNIFUZZ_EXECUTOR_FORKSRV_EXECUTOR_H_H

#include <iostream>
#include <string>
#include <vector>

#include "omnifuzz/executor/executor.h"
#include "omnifuzz/executor/fault.h"
#include "omnifuzz/executor/file_descriptor.h"
#include "omnifuzz/executor/forkserver.h"
#include "omnifuzz/executor/fork_client.h"
#include "omnifuzz/feedback/mechanism.h"

namespace omnifuzz {

// maybe there's a better name
class ForkServerExecutor: public Executor {
 public:
  // Not sure the use case, but we allow the executable comes in later.
  ForkServerExecutor();
  ForkServerExecutor(std::vector<std::string>, 
                     FeedbackMechanism*);
  virtual ~ForkServerExecutor();
  virtual bool Initialize(char**, FeedbackMechanism*) override;
  virtual bool Initialize(std::vector<std::string>,
                          FeedbackMechanism*) override;
  virtual bool CaptureCrash(void) override;
  virtual void Execute(uint8_t*, size_t) override;
 private:
  Forkserver* fsrv_;
  ForkClient* fclnt_;
  CommonFD fd_;
  int curr_exit_status_;
};
} // namespace omnifuzz
#endif  // OMNIFUZZ_EXECUTOR_FORKSRV_EXECUTOR_H_H

