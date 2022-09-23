#include "testcase_manager.h"

namespace omnifuzz {

TestcaseFileManager::TestcaseFileManager() {
    
}

TestcaseFileManager::~TestcaseFileManager() {

}


void TestcaseFileManager::ReloadTestcaseFiles(std::string input_dir_path) {
   
  if (!std::filesystem::exists(input_dir_path)) {
    std::cerr << "[Error] Directory does not exist." << std::endl;
    return;
  }
  
  for (auto &file: std::filesystem::directory_iterator(input_dir_path)) {
    Testcase testcase;
    testcase.file_name = file;
    testcase.size = GetFileSize(file); 
    testcase.exec_us = 0;
    testcase.generation = 0;
    scheduler_->Enqueue(testcase);
  }
}


void TestcaseFileManager::CreateCrashReport() {

}


void TestcaseFileManager::BuildDirectoryTree(void) {

}

size_t TestcaseFileManager::GetFileSize(std::string file_name) {
  std::ifstream ifs(file_name, std::ifstream::ate|std::ifstream::binary);
  return ifs.tellg();
}


} // namespace omnifuzz
