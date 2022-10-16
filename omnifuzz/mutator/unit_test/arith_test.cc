#include <stdio.h>
#include <string.h>

#include <iostream>
#include <cstdint>

#include "../mutator.h"
#include "../arithmetic.h"
using namespace std;

class AllArithMutator: public omnifuzz::MutatorComposite {
 public:
   AllArithMutator() {
     AddMutator(new omnifuzz::ArithmeticMutator(omnifuzz::MutationGranularity::kByte));
     AddMutator(new omnifuzz::ArithmeticMutator(omnifuzz::MutationGranularity::kWord));
     // AddMutator(new omnifuzz::ArithmeticMutator(omnifuzz::MutationGranularity::kDword));
   }
};



size_t kBufSize = 8;
int main(int argc, char** argv) {
  uint8_t buf[kBufSize];
  AllArithMutator mutator;

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
