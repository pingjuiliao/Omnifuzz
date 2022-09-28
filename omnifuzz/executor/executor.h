#ifndef OMNIFUZZ_EXECUTOR_EXECUTOR_H
#define OMNIFUZZ_EXECUTOR_EXECUTOR_H

#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <iostream>
#include <string>
#include <vector>

#include "omnifuzz/feedback/feedback_mechanism.h"

namespace omnifuzz {

class Executor {
 public:
  virtual ~Executor();

  virtual bool Initialize(char**, FeedbackMechanism*);
  // Executor sometimes needs to wait for preprocessor so we cannot always 
  // load up executable in constructor
  virtual bool Initialize(std::vector<std::string>, 
                          FeedbackMechanism*) = 0;
  virtual void Execute(char*, size_t) = 0;
  virtual void AllocateFeedbackData(void);
  virtual void* DumpFeedbackData(void);
  const char* kShmEnv = "OMNIFUZZ_SHM_ENV";
 protected:
  virtual void ParseArgument(char**);
  FeedbackMechanism* fdbk_mech_;
  int shm_id_;
  void* feedback_data_ptr_;
  char** c_argv_;
};
} // namespace omnifuzz

#endif  // OMNIFUZZ_EXECUTOR_EXECUTOR_H
