#ifndef OMNIFUZZ_EXECUTOR_FILE_DESCRIPTOR_H
#define OMNIFUZZ_EXECUTOR_FILE_DESCRIPTOR_H

struct CommonFD {
  int dev_null;
  int dev_urandom;
  int out_file;
};

#endif  // OMNIFUZZ_EXECUTOR_FILE_DESCRIPTOR_H
