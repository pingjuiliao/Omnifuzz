
#include "omnifuzz/executor/executor.h"

namespace omnifuzz {

Executor::~Executor() {
  // TODO: handle non-heap memory 
  if (c_argv_) {
    char** p = c_argv_;
    while (*p) {
      free(*p);
      p++;
    } 
    free(c_argv_);
  }
  shmctl(shm_id_, IPC_RMID, NULL);
}

bool Executor::Initialize(char** argv, 
                          FeedbackMechanism* fdbk_mech) {
  if (!argv) {
    return false;
  }
  fdbk_mech_ = fdbk_mech;
  AllocateFeedbackData();
  ParseArgument(argv); 
  
  return true;
}

void Executor::ParseArgument(char** argv) {
  if (!argv) {
    c_argv_ = nullptr;
    return;
  }
  char** p = argv;
  size_t arg_size = 0;
  while (*p) {
    p++;
    arg_size++;
  }
  c_argv_ = (char **) malloc(sizeof(char *) * (arg_size + 1));
  for (int i = 0; i < arg_size; ++i) {
    c_argv_[i] = strdup(argv[i]); 
  }
  c_argv_[arg_size] = nullptr;
}


void Executor::AllocateFeedbackData(void) {
  if (!fdbk_mech_) {
    return;
  }
  // shmget
  size_t data_size = fdbk_mech_->RegisterFeedbackData();
  shm_id_ = shmget(IPC_PRIVATE, data_size, IPC_CREAT|IPC_EXCL| 0600);
  if (shm_id_ < 0) {
    std::cerr << "[FATAL] Error in creating SHM." << std::endl;
  }
  std::cout << "shm_id is: " << shm_id_ << std::endl;
  
  // shmat
  feedback_data_ptr_ = shmat(shm_id_, NULL, 0);
  
  if (!feedback_data_ptr_) {
    std::cerr << "[FATAL] Error in tracing SHM." << std::endl;
  }
  
  // Embedded executor must know 
  std::string shm_str = std::to_string(shm_id_);
  setenv(kShmEnv, shm_str.c_str(), 1);
}

void* Executor::DumpFeedbackData(void) {
  return feedback_data_ptr_;
}

} // namespace omnifuzz
