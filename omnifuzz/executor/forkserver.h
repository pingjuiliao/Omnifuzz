#ifndef OMNIFUZZ_EXECUTOR_FORK_SERVER_H
#define OMNIFUZZ_EXECUTOR_FORK_SERVER_H

#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdint>
#include <cstdlib>
#include <iostream>

#include "omnifuzz/executor/file_descriptor.h"
#include "omnifuzz/feedback/feedback_mechanism.h"

namespace omnifuzz {

/*
 * Not a singleton: fuzzing in different start point(basic block) 
 * is interesting
 */
class Forkserver {
 public:
  
  Forkserver(char**, CommonFD);
  virtual ~Forkserver(); 
  
  // ForkClient access data member while connecting
  friend class ForkClient;

 protected:
  bool RedirectServerFD(void);  
  bool Initialize(void);
  void ExecuteEmbeddedForkserver();
 private:
  
  char** argv_; 
  pid_t forksrv_pid_;
  bool is_initialized_;
  
  // file descriptors that should be managed
  CommonFD common_fd_;
  // pipe for communicate
  int request_pipe_[2]; 
  int response_pipe_[2];
  const int kRequestFD = 21;
  const int kResponseFD = 22;

  // state
  bool file_mode;

  // For testing
 public:
  inline static int testing_shm_id;
  static void SendInitResponse(int response_fd);
  static bool ReceiveClientRequest(int request_fd);
  static bool ServeRequest(int request_fd, int response_fd);
};

} // namespace omnifuzz
#endif  // OMNIFUZZ_EXECUTOR_FORKSERVER_H
