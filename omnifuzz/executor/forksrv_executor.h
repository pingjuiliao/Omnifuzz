#ifndef FCE_H
#define FCE_H

#include <iostream>
#include <string>
#include <vector>

#include "executor.h"
#include "file_descriptor.h"
#include "forkserver.h"
#include "fork_client.h"

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
#endif  // 
