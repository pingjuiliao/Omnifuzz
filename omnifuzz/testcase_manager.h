#ifndef TESTCASE_MANAGER_H
#define TESTCASE_MANAGER_H

#include <fstream>
#include <filesystem>
#include <iostream>

#include "testcase_pool.h"

#include "llvm/Transform/Omnifuzz/Feedback/feedback.h"


namespace omnifuzz {

class TestcaseManager {
 public:
  TestcaseManager();
  ~TestcaseManager();
  void ReloadTestcases(std::string);
  void WriteCrashReport(Testcase&);
  void WriteTestcaseReport(Testcase&);
 protected:
  bool TestcaseIsDuplicated(Testcase&);
 private:
  std::unordered_set<std::string> processed_testcases_;
  TestcasePool testcase_pool_;
};


} // namespace omnifuzz
#endif  // TESTCASE_MANAGER_H
