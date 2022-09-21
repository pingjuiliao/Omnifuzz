#ifndef FUZZER_EXECUTOR_FORK_CLIENT_H
#define FUZZER_EXECUTOR_FORK_CLIENT_H

#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#include <cstdint>

namespace omnifuzz {

#pragma pack(push, 1)
struct ForkRequest {
  const char signature[4];
  uint32_t flag;
};  
#pragma pack(pop)

class ForkClient {
 public:
  ForkClient();
  virtual ~ForkClient();
  pid_t SendRequest();
 private:
  pid_t pid_;
};

} // namespace omnifuzz
#endif  // FUZZER_EXECUTOR_FORK_CLIENT_H
