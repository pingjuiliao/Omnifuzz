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

// TestcaseFileManager talks to the filesystem
// composite this for any fuzz component that talks to the filesystem
class TestcaseFileManager {
 public:
  TestcaseFileManager();
  ~TestcaseFileManager();
  // bool TestcaseIsDuplicated(Testcase&);

  bool BuildDirectoryTree(void);
  bool BuildDirectoryTree(std::string);
  void CreateCrashReport(uint8_t*, size_t);
  void CreateTestcaseFile(Testcase&, uint8_t*, size_t);

  // Load Seed testcases
  bool LoadSeedTestcaseFiles(Scheduler*, std::string);

  // Buffer management: An alternative is to move these function to 
  // omnifuzz::Mutator static
  uint8_t* LoadToBuffer(Testcase*);
  void Unload(uint8_t*);

  // Reload all the testcases that has been discovered.
  // This should be rare operation, example usages are:
  //  - SpliceMutator.
  // std::vector<Testcase*> ReloadTestcaseFiles();

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
