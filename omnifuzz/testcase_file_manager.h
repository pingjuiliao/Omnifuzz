#ifndef OMNIFUZZ_TESTCASE_FILE_MANAGER_H
#define OMNIFUZZ_TESTCASE_FILE_MANAGER_H

#include <cstring>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <limits>

#include "omnifuzz/scheduler/scheduler.h"
#include "omnifuzz/testcase.h"

// The string to std::filesystem::path works fine.
namespace omnifuzz {

struct FuzzOutputDirectory {
  std::string root;
  std::string testcase_dir;
  std::string crash_dir;
};

class TestcaseFileManager {
 public:
  TestcaseFileManager();
  ~TestcaseFileManager();
  // bool TestcaseIsDuplicated(Testcase&);

  bool BuildDirectoryTree(void);
  bool BuildDirectoryTree(std::string);
  void CreateCrashReport(uint8_t*, size_t);
  void CreateTestcaseFile(Testcase&, uint8_t*, size_t);
  bool LoadSeedTestcaseFiles(Scheduler*, std::string);

  uint8_t* LoadToBuffer(Testcase*);
  void Unload(uint8_t*);
  // TODO: This should be more complicated than just an ID.
  //  filename is a good place to store information.
  //  also, it might want to takes more arguments in.
  std::filesystem::path NameTestcase();
 private:
  // std::unordered_set<std::string> processed_testcases_;
  FuzzOutputDirectory* out_dir_;
  uint32_t num_seeds_ = 0;
};


} // namespace omnifuzz
#endif  // OMNIFUZZ_TESTCASE_FILE_MANAGER_H
