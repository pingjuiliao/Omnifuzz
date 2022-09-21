#ifndef OMNIFUZZ_PREPROCESSOR_INSTRUMENTATOR_H
#define OMNIFUZZ_PREPROCESSOR_INSTRUMENTATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


namespace omnifuzz {

class Instrumentator {
 public:
  Instrumentator();
  Instrumentator(const char*);
  ~Instrumentator();
  int Instrument(const char*);
 protected:
  int Execute(const char*, char**);
  bool HandleTargetName(const char*, char**, char**, char**);
  bool FileExist(const char*);
 private:
  char* llvm_build_root_path_;
  char* clang_path_;
  char* opt_path_;
};

} // namespace omnifuzz

#endif  // OMNIFUZZ_PREPROCESSOR_IUNSTRUMENTATOR_H
