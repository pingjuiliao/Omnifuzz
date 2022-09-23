#ifndef OMNIFUZZ_EXECUTOR_FORK_CLIENT_H
#define OMNIFUZZ_EXECUTOR_FORK_CLIENT_H

#include <fcntl.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdint>
#include <string>
#include <vector>

#include "omnifuzz/executor/executor.h"
#include "omnifuzz/executor/forkserver.h"

namespace omnifuzz {

class ForkClient final {
 public:
  ForkClient(): fsrv_(nullptr) {}; 
  ~ForkClient();
  void Connect(Forkserver*);
  void SendRequest(void);
 private:
  pid_t pid_;
  int request_fd_;
  int response_fd_;
  Forkserver* fsrv_;
  CommonFD common_fd_;
};

} // namespace omnifuzz

#endif  // OMNIFUZZ_EXECUTOR_FORK_CLIENT_H
