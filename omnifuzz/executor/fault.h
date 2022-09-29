#ifndef OMNIFUZZ_EXECUTOR_FAULT_H
#define OMNIFUZZ_EXECUTOR_FAULT_H
enum Fault {
  kNone=0, 
  kTmout, 
  kCrash, 
  kError,
  kNoinst, 
  kNobits,
};
#endif  // OMNIFUZZ_EXECUTOR_FAULT_H
