#include <stdio.h>
#include <string.h>

#include <iostream>
#include <cstdint>

#include "omnifuzz/mutator/mutator.h"
#include "omnifuzz/mutator/interesting.h"
using namespace std;

class AllInterestMutator: public omnifuzz::MutatorComposite {
 public:
   AllInterestMutator() {
     AddMutator(new omnifuzz::InterestingInt8Mutator());
     AddMutator(new omnifuzz::InterestingInt16Mutator());
     AddMutator(new omnifuzz::InterestingInt32Mutator());
   }
};

size_t kBufSize = 8;
int main(int argc, char** argv) {
  uint8_t buf[kBufSize];
  AllInterestMutator mutator;

  memset((char *)buf, 0, kBufSize);
  omnifuzz::MutationResult r = omnifuzz::MutationResult::kNumResults;
  int i = 0;
  while (r != omnifuzz::MutationResult::kCycleDone) {
    r = mutator.Mutate(buf, kBufSize);
    if (r == omnifuzz::MutationResult::kAbort) {
      cerr << "[ABORT] on round " << i << endl;
    }
    for (size_t i = 0; i < kBufSize; ++i) {
      printf("%02hhx ", buf[i]);
    }
    printf("\n"); 
    i ++;
  }

  return 0;
}
