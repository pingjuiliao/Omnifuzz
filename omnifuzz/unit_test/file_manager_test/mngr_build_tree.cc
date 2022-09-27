#include <cstdint>
#include <iostream>

#include "omnifuzz/testcase_file_manager.h"
#include "omnifuzz/testcase.h"
#include "omnifuzz/scheduler/scheduler.h"

class DummyScheduler: public omnifuzz::Scheduler {
 public:
  DummyScheduler(): cnt_(0) {}
  virtual void Enqueue(omnifuzz::Testcase t) override {
    std::cout << "[Message] Testcase scheduled!" << std::endl \
              << "file name: " << t.file_name << std::endl \
              << "size: " << t.size << std::endl \
              << "#testcases: " << ++cnt_ << std::endl << std::endl;
  }
  virtual omnifuzz::Testcase* Dequeue(void) override {
    return new omnifuzz::Testcase;
  }
 private:
  uint32_t cnt_;
};

int main(int argc, char** argv) {
  DummyScheduler scheduler;
  omnifuzz::TestcaseFileManager mngr;
  mngr.BuildDirectoryTree();
  mngr.LoadSeedTestcaseFiles(&scheduler, "./omnifuzz_in");
  return 0;
}
