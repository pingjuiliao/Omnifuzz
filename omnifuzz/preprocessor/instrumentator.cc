#include "omnifuzz/preprocessor/instrumentator.h"

namespace omnifuzz {

Instrumentator::Instrumentator() {
  Instrumentator("/bin/");
}

Instrumentator::Instrumentator(const char* llvm_path) {
  llvm_build_root_path_ = strdup(llvm_path);
  static const char* kOpt = "/llvm-project/llvm/bin/opt";
  static const char* kClang = "/llvm-project/llvm/bin/clang";
  const size_t kBufSize = 0x100;

  // prepare executable: clang
  clang_path_ = (char *) malloc(kBufSize);
  memset(clang_path_, 0, kBufSize);
  strcpy(clang_path_, llvm_build_root_path_);
  strcat(clang_path_, kClang);
  
  // prepare executable: opt
  opt_path_ = (char *) malloc(kBufSize);
  memset(opt_path_, 0, kBufSize);
  strcpy(opt_path_, llvm_build_root_path_);
  strcat(opt_path_, kOpt);
}

Instrumentator::~Instrumentator() {
  free(llvm_build_root_path_);
  free(clang_path_);
  free(opt_path_);
}

int Instrumentator::Instrument(const char* target) {
  
  char* target_ir;
  char* target_bitcode;
  char* target_binary;

  if (!FileExist(target)) {
    fprintf(stderr, "[ERROR] target file %s does not exist.\n", target);
    return -1;
  }  

  HandleTargetName(target, &target_ir, &target_bitcode, &target_binary);
  puts(target_ir);
  puts(target_bitcode);
  puts(target_binary);
  
  // Source code to .ll file
  const char* const kEmitLlvmArgv[] = {
    clang_path_, "-o", "a.ll", "-O1", "-m64", "-emit-llvm", "-S", target, NULL, 
  };
  Execute(clang_path_, (char**)kEmitLlvmArgv);
  
  // Optimization: .ll to .bc file
  const char* const kOptimizeArgv[] = {
    opt_path_, "-o", "a.bc", "-passes=omnifuzz-afl", "a.ll", NULL,    
  };
  Execute(opt_path_, (char**)kOptimizeArgv);

  // Bitcode to executable binary 
  const char* const kCompileArgv[] = {
    clang_path_, "-o", "a.exe", "a.bc", NULL, 
  };
  Execute(clang_path_, (char**)kCompileArgv); 

  return 0;
}


int Instrumentator::Execute(const char* executable, char* argv[]) {
  pid_t tpid = -1;
  pid_t pid = fork();
  int child_status = -1;
  if (pid == 0) {
    execvp(executable, &argv[0]);
    fprintf(stderr, "[ERROR] Unknown command\n");
    exit(-1);
  } 
  do {
    tpid = waitpid(pid, &child_status, WIFEXITED(WNOHANG));
  } while (tpid != pid);
  return child_status;  
}

bool Instrumentator::HandleTargetName(const char* source, 
                                      char** ir, 
                                      char** bitcode,
                                      char** executable_binary) {
  char* common_prefix = NULL;
  char* c_ptr = strrchr((char *)source, '.');
  size_t common_prefix_size = 0;
  if (c_ptr) {
    common_prefix_size = c_ptr - source;
    common_prefix = (char *) malloc(common_prefix_size); 
    memset(common_prefix, 0, common_prefix_size);
    strncpy(common_prefix, source, common_prefix_size);
  } else {
    common_prefix = strdup(source);
  }
  
  const size_t buf_size = common_prefix_size + 5;
  *ir = (char *) malloc(buf_size);
  memset(*ir, 0, buf_size);
  strcpy(*ir, common_prefix);
  strcat(*ir, ".ll");

  *bitcode = (char *) malloc(buf_size);
  memset(*bitcode, 0, buf_size);
  strcpy(*bitcode, common_prefix);
  strcat(*bitcode, ".bc");

  *executable_binary = (char *) malloc(buf_size);
  memset(*executable_binary, 0, buf_size);
  strcpy(*executable_binary, common_prefix);
  strcat(*executable_binary, ".exe");

  return true;
}

bool Instrumentator::FileExist(const char* path) {
  
  if (path[0] == '/') {
    return access(path, F_OK) == 0;
  }

  const size_t buf_size = 0x100;
  char* absolute_path = (char *)malloc(buf_size);
  memset(absolute_path, 0, buf_size);

  char* workdir_ptr = getcwd(absolute_path, buf_size);
  if (!workdir_ptr) {
    fprintf(stderr, "[ERROR] cannot get working directorey");
    return false;
  }
  strcat(absolute_path, "/");
  strcat(absolute_path, path);
  printf("Checking %s\n", absolute_path);
  return access(absolute_path, F_OK) == 0;
}


} // namespace omnifuzz
