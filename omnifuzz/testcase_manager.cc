#include "testcase_manager.h"

namespace omnifuzz {

TestcaseManager::TestcaseManager() {
    
}

TestcaseManager::~TestcaseManager() {

}

void TestcaseManager::ReloadTestcases(std::string dir_path) {
   
  if (!std::filesystem::exists(path)) {
    std::cerr << "[Error] Directory does not exist." << std::endl;
    return;
  }
  
  for (auto &file: std::filesystem::directory_iterator(dir_path)) {
    std::string testcase_file = std::filesystem::absolute(file);
    testcase = new Testcase(testcase_file);
    testcase_pool.Add(testcase);
  }


}

} // namespace omnifuzz
