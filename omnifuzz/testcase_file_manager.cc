#include "omnifuzz/testcase_file_manager.h"

namespace omnifuzz {

TestcaseFileManager::TestcaseFileManager() {
}

TestcaseFileManager::~TestcaseFileManager() {

}

// Create out_dir at relative path ./omnifuzz_out 
bool TestcaseFileManager::BuildDirectoryTree(void) {
  return BuildDirectoryTree("omnifuzz_out");
}

// Create Output directory at given path
bool TestcaseFileManager::BuildDirectoryTree(std::string root_dir) {
  if (std::filesystem::exists(root_dir)) {
    std::cerr << "[ERROR]" << root_dir << " already exists" \
              << std::endl;
    return false;
  }
  out_dir_ = new FuzzOutputDirectory;
  out_dir_->root = root_dir;
  out_dir_->testcase_dir = root_dir + "/testcase";
  out_dir_->crash_dir = root_dir + "/crash";

  std::filesystem::create_directory(out_dir_->root);
  std::filesystem::create_directory(out_dir_->testcase_dir);
  std::filesystem::create_directory(out_dir_->crash_dir);
  // TODO more file should be created; 

  return true;
} 

// Make a crash report
void TestcaseFileManager::CreateCrashReport(char* buf, size_t size) {
  static uint32_t crash_count = 0;
  std::ofstream ofs;
  std::string file_name = "/id_" + std::to_string(crash_count++);
  ofs.open(out_dir_->testcase_dir + file_name);
  ofs.write(buf, size); 
  ofs.close();
}

// 
void TestcaseFileManager::CreateTestcaseFile(Testcase &testcase, 
                                             char* buf, size_t size) {
  std::ofstream ofs;
  std::string file_name = NameTestcase();
  testcase.file_name = file_name.c_str();
  ofs.open(file_name);
  ofs.write(buf, size);
  ofs.close();
}

// 
bool TestcaseFileManager::LoadSeedTestcaseFiles(Scheduler* scheduler,
                                                std::string input_dir_path) {
   
  if (!scheduler) {
    return false;
  }
  
  if (!std::filesystem::exists(input_dir_path)) {
    std::cerr << "[Error] Directory does not exist." << std::endl;
    return false;
  }

  if (!out_dir_) {
    return false;
  }
  
  // TODO: Ignore some filenames, e.g. .ignore, core, ...
  for (auto &file: std::filesystem::directory_iterator(input_dir_path)) {
    if (std::filesystem::is_directory(file)) {
      continue;
    }
    // Handle names and type conversion.
    std::string in_fpath = file.path().string();
    std::string in_fname = in_fpath.substr(in_fpath.find_last_of("/") + 1);
    std::string out_fname = "/seed_" + in_fname;
    std::string out_fpath = out_dir_->testcase_dir + out_fname;

    // Create a Testcase
    Testcase testcase;
    testcase.file_name = out_fname.c_str();
    testcase.size = std::filesystem::file_size(file); 
    testcase.exec_us = 0;
    testcase.generation = 0;
    scheduler->Enqueue(testcase);
    // we simply copy the seed testcases;
    std::filesystem::copy(file, out_fpath);
  }
  return true;
}


} // namespace omnifuzz
