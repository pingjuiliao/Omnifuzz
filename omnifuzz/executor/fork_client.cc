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
  
  if (status != 0x74696e69) {
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
  int pid;
  int status;
  
  if (write(request_fd_, &code, sizeof(int)) < 4) {
    std::cerr << "[Client] unable to write to request pipe" << std::endl;
    return;
  }
  
  if (read(response_fd_, &pid, 4) < 4) {
    std::cerr << "[Client] unable to read the \"pid\" response" << std::endl;
    return;
  }
  std::cout << "[Client] Pid received: knowing " << pid << " is executing."
            << std::endl;

  if (read(response_fd_, &status, 4) < 4) {
    std::cerr << "[Client] unable to read the \"status\" response" << std::endl;
    return;
  }
  std::cout << "[Client] Pid exited: exit code:" << status << std::endl;
}

Fault ForkClient::ReportFault(void) {
  if (!curr_exit_status_) {
    return Fault::kNone;
  }
  if (WIFSIGNALED(curr_exit_status_)) {
    return Fault::kCrash;
  }
}

} // namespace omnifuzz
