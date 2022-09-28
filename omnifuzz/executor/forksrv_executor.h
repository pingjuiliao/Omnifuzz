#ifndef OMNIFUZZ_EXECUTOR_FORKSRV_EXECUTOR_H_H
#define OMNIFUZZ_EXECUTOR_FORKSRV_EXECUTOR_H_H

#include <iostream>
#include <string>
#include <vector>

#include "omnifuzz/executor/executor.h"
#include "omnifuzz/executor/file_descriptor.h"
#include "omnifuzz/executor/forkserver.h"
#include "omnifuzz/executor/fork_client.h"
#include "omnifuzz/feedback/feedback_mechanism.h"

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
  virtual void Execute(char*, size_t) override;
 private:
  Forkserver* fsrv_;
  ForkClient* fclnt_;
  CommonFD fd_;
};
} // namespace omnifuzz
#endif  // OMNIFUZZ_EXECUTOR_FORKSRV_EXECUTOR_H_H

