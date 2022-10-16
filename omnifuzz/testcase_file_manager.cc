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

  return true;
} 

// Make a crash report
void TestcaseFileManager::CreateCrashReport(uint8_t* buf, size_t size) {
  static uint32_t crash_count = 0;
  std::ofstream ofs;
  std::string file_name = "/id_" + std::to_string(crash_count++);
  ofs.open(out_dir_->testcase_dir + file_name);
  ofs.write((char *)buf, size); 
  ofs.close();
}

std::filesystem::path TestcaseFileManager::NameTestcase() {
    static uint32_t id = 0;
    return "/tid_" + std::to_string(id++);
}
// 
void TestcaseFileManager::CreateTestcaseFile(Testcase &testcase, 
                                             uint8_t* buf, size_t size) {
  // name the testcase
  std::ofstream ofs;
  std::string file_name = NameTestcase();
  testcase.file_name = strdup(file_name.c_str());
  testcase.size = size;
  
  std::string file_path = out_dir_->testcase_dir + file_name; 
  if (std::filesystem::exists(file_path)) {
    std::cerr << "Cannot add file to filepath\n";
    return;
  }
  ofs.open(file_path);
  ofs.write((char *)buf, size);
  ofs.close();
  
  std::cout << "[TestcaseFileMngr]: testcase added to the filesystem" \
            << "\n\t\t\tfile_name: " << testcase.file_name \
            << "\n\t\t\t  size   : " << testcase.size \
            << "\n\t\t\t exec_us : " << testcase.exec_us << std::endl;
}

// 
bool TestcaseFileManager::LoadSeedTestcaseFiles(Scheduler* scheduler,
                                                std::string input_dir_path) {
   
  if (!scheduler) {
    std::cerr << "[Error] does not have any scheduler" << std::endl;
    return false;
  }
  
  if (!std::filesystem::exists(input_dir_path)) {
    std::cerr << "[Error] Directory does not exist." << std::endl;
    return false;
  }

  if (!out_dir_) {
    std::cerr << "[Error] does not have any out put directory" << std::endl;
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
    testcase.file_name = strdup(out_fname.c_str());
    testcase.size = std::filesystem::file_size(file); 
    testcase.was_fuzzed = 0;
    testcase.exec_us = 0;
    testcase.generation = 0;
    scheduler->Enqueue(testcase);
    
    // we simply copy the seed testcases;
    std::filesystem::copy(file, out_fpath);
  }
  std::cout << "[TestcaseFileMngr]: seeds loaded" << std::endl;
  
  return true;
}

uint8_t* TestcaseFileManager::LoadToBuffer(Testcase* testcase) {
  if (testcase->size == 0) {
    std::cerr << "[TFM] testcase has size 0" << std::endl;
    return nullptr;
  }
  std::string testcase_path = out_dir_->testcase_dir + testcase->file_name;
  if (!std::filesystem::exists(testcase_path)) {
    std::cerr << "[TestcaseFileManager] cannot find the testcase: "
              << testcase_path << std::endl;
    return nullptr;
  }

  uint8_t* buffer = new uint8_t[testcase->size * 2];
  std::ifstream ifs;
  ifs.open(testcase_path);
  if (!ifs.is_open()) {
    std::cerr << "[TestcaseFileManager]: Cannot open file" 
              << testcase_path << std::endl;
    return nullptr;
  }
  memset(buffer, 0, sizeof(buffer));
  ifs.read((char *) buffer, testcase->size);
  ifs.close();
  return buffer;
}



void TestcaseFileManager::Unload(uint8_t* buffer) {
  delete[] buffer;
}

} // namespace omnifuzz
