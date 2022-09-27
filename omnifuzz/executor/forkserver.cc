
#include "omnifuzz/executor/forkserver.h"

namespace {

void ManageResources(void) {
  // TODO 
}
} // anonymous namespace  

namespace omnifuzz {

Forkserver::Forkserver(char** argv, CommonFD fd): argv_(argv), common_fd_(fd) {
  is_initialized_ = Initialize();
  if (!is_initialized_) {
    std::cerr << "[FATAL] Cannot initialize the Forkserver";
  }
}

Forkserver::~Forkserver() {
 
}


bool Forkserver::Initialize() {
  
  if (pipe(request_pipe_) || pipe(response_pipe_)) {
    std::cerr << "[FATAL] Failed on pipe()" << std::endl;
    return false;
  }
  
  forksrv_pid_ = fork();
  if (forksrv_pid_ < 0) 
    return false;
  
  if (!forksrv_pid_) {
    // before execve
    ManageResources();
    if (!RedirectServerFD()) {
      std::cerr << "Failed on redirection of FDs" << std::endl;
      return false;
    }
    ExecuteEmbeddedForkserver();
    return false;
  }

  close(request_pipe_[0]);
  close(response_pipe_[1]);

  return true;
}

bool Forkserver::RedirectServerFD(void) {

  if (file_mode) {
    dup2(common_fd_.dev_null, 0); 
  } else {
    dup2(common_fd_.out_file, 0);
    close(common_fd_.out_file);
  }
#ifndef UNIT_TEST
  dup2(common_fd_.dev_null, 1);
  dup2(common_fd_.dev_null, 2);
#endif
  
  if (dup2(request_pipe_[0], kRequestFD) < 0) {
    std::cerr << "[FATAL] Failed on dup2()" << std::endl;
    return false;
  }

  if (dup2(response_pipe_[1], kResponseFD) < 0) {
    std::cerr << "[FATAL] Failed on dup2()" << std::endl;
    return false;
  }

  close(request_pipe_[0]);
  close(request_pipe_[1]);
  close(response_pipe_[0]);
  close(response_pipe_[1]);

  close(common_fd_.dev_null);
  close(common_fd_.dev_urandom);
  return true;
}

void Forkserver::ExecuteEmbeddedForkserver(void) {
  execv(argv_[0], argv_);  
}

// In most cases, these static function will not be in fuzzing cycles
void Forkserver::SendInitResponse(int response_fd) {
  const uint32_t code = 0x74696e69; // "init"

  // Shm mechanism works first
  char* s = getenv("OMNIFUZZ_SHM_ENV");
  if (!s) {
    std::cerr << "[ERROR] Failed on getenv()" << std::endl;
  }
  testing_shm_id = atoi(s);
  std::cout << "[Server] Get SHM id: " << testing_shm_id << std::endl;

  // pipe works later
  if (write(response_fd, &code, sizeof(code)) < 4) {
    std::cerr << "[ERROR] Failed on write()" << std::endl;
  }
}

bool Forkserver::ReceiveClientRequest(int request_fd) {
  int status = 0;
  // Commanding pipe
  read(request_fd, &status, 4);
  if (!status) {
    std::cout << "[ERROR] Cannot Receive Client request" << std::endl;
    return false;
  }
  return true;
}

bool Forkserver::ServeRequest(int request_fd, int response_fd) {
  pid_t child_pid;
  int status;
  child_pid = fork();
  if (child_pid < 0) {
    std::cerr << "[ERROR] Failed on fork()" << std::endl;
    exit(-1);
  }

  if (!child_pid) {
    char* buf = (char *) shmat(testing_shm_id, NULL, 0);
    sleep(3);
    sprintf(buf, "Hello, I am a forked instance.");
    exit(0);
  }
   
  write(response_fd, &child_pid, 4);
  waitpid(child_pid, &status, WIFEXITED(WNOHANG));
  write(response_fd, &status, 4);
  return true;
}


} // namespace omnifuzz
