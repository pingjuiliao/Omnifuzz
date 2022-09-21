#include "forksrv_executor.h"

namespace omnifuzz {

ForkServerExecutor::ForkServerExecutor() {
  fsrv_ = nullptr;
  fclnt_ = nullptr;
}

ForkServerExecutor::ForkServerExecutor(std::vector<std::string> argv) {
  if (!LoadExecutable(argv)) {
    std::cerr << "[FATAL] Cannot load executable" << std::endl;
    throw "Cannot Load executable";
  }
}

ForkServerExecutor::~ForkServerExecutor() {
  delete fsrv_;
  delete fclnt_;
}


bool ForkServerExecutor::LoadExecutable(std::vector<std::string> argv) {
 
  // construct c_argv_ 
  LoadArgument(argv);  
  if (!c_argv_) {
    std::cerr << "[FATAL] Cannot load arguments" << std::endl;
  }

  // fd: /dev/null
  fd_.dev_null = open("/dev/null", O_RDWR);
  if (fd_.dev_null < 0) {
    std::cerr << "[FATAL] Failed on open() /dev/null" << std::endl;
    return false;
  }
  
  // fd: dev_urandom
  fd_.dev_urandom = open("/dev/urandom", O_RDONLY);
  if (fd_.dev_urandom < 0) {
    std::cerr << "[FATAL] Failed on open() /dev/urandom" << std::endl;
    return false;
  }

  // fd: out_file
  const char* kTmpFile = "./.cur_input";
  fd_.out_file = open(kTmpFile, O_RDWR | O_CREAT| O_EXCL, 0600);
  if (fd_.out_file < 0) {
    std::cerr << "[FATAL] Failed on open() " << kTmpFile << std::endl;
  }
  
  // Everytime we instantiate an forkserver
  if (fsrv_) {
    // fsrv_.SaveToBackUp();
    std::cerr << "[ERROR] Multiple forkservers not supported" << std::endl;
    return false;
  }

  fsrv_ = new Forkserver(c_argv_, fd_);
  fclnt_ = new ForkClient();
  fclnt_->Connect(fsrv_);

  return true;
}


void ForkServerExecutor::Execute(void) {
  fclnt_->SendRequest();
}

} // namespace omnifuzz
