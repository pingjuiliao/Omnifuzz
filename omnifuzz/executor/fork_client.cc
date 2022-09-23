#include "omnifuzz/executor/fork_client.h"

namespace {

void MapFDs() {
  
}

} // anonymous namespace


namespace omnifuzz {

ForkClient::~ForkClient() {}

void ForkClient::Connect(Forkserver* fsrv) {
  if (!fsrv) {
    std::cerr << "[ERROR] Trying to connect nothing." << std::endl;
  }
  fsrv_ = fsrv;
  if (!fsrv_->is_initialized_) {
    std::cerr << "[ERROR] Connection failed" << std::endl;
    fsrv_ = nullptr;
  }
  common_fd_ = fsrv->common_fd_;
  request_fd_ = fsrv_->request_pipe_[1];
  response_fd_ = fsrv_->response_pipe_[0];

  int status = 0;
  if (read(response_fd_, &status, sizeof(int)) < 4) {
    std::cerr << "[ERROR] read() in connection failed" << std::endl;
  }
  
  // 
  if (status == 0x74696e69) {
  #ifndef UNIT_TEST
    std::cout << "[MESSAGE] Connection Succeeded!" << std::endl;
  #else 
    std::cout << "[Client] Receive Initialization code" << std::endl \
              << std::endl << std::endl;
  #endif
  }
}

void ForkClient::SendRequest(void) {
  if (!fsrv_) 
    return;
  int code = 2020;
  int res;
  
  if (write(request_fd_, &code, sizeof(int)) < 4) {
    std::cerr << "[Client] unable to write to request pipe" << std::endl;
    return;
  }
  

  read(response_fd_, &res, 4);
  if (res == code) {
    std::cout << "[Client] Response received!" << std::endl;
    std::cout << "[Message] Cycle completed!" << std::endl;
    std::cout << std::endl << std::endl;
  }
}
} // namespace omnifuzz
