#ifndef TESTCASE_MANAGER_H
#define TESTCASE_MANAGER_H

#include <fstream>
#include <filesystem>
#include <iostream>
#include <limits>


#include "testcase_pool.h"
#include "omnifuzz/scheduler/scheduler.h"
#include "llvm/Transform/Omnifuzz/Feedback/feedback.h"


namespace omnifuzz {

class TestcaseFileManager {
 public:
  TestcaseFileManager();
  ~TestcaseFileManager();
  void ReloadTestcaseFiles(std::string);
  // bool TestcaseIsDuplicated(Testcase&);
  void CreateCrashReport(void);
  void BuildDirectoryTree(void);
  void CreateTestcaseFile(Testcase);
 private:
  uint32_t GetFileSize(std::string);
  std::unordered_set<std::string> processed_testcases_;
};


} // namespace omnifuzz
#endif  // TESTCASE_MANAGER_H
