
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <cstdint>

#include "../mutator.h"
#include "../bit_flip.h"

const size_t kBufSize = 4;

class AllBitFlipMutator: public omnifuzz::MutatorComposite {
 public:
  AllBitFlipMutator() {
    AddMutator(new omnifuzz::BitFlipMutator(omnifuzz::MutationGranularity::kBit));
    AddMutator(new omnifuzz::BitFlipMutator(omnifuzz::MutationGranularity::kBits2));
    AddMutator(new omnifuzz::BitFlipMutator(omnifuzz::MutationGranularity::kBits4));
    AddMutator(new omnifuzz::BitFlipMutator(omnifuzz::MutationGranularity::kByte));
  }
};


int main(int argc, char** argv) {
  uint8_t buf[kBufSize];
  
  AllBitFlipMutator mutator;
  
  memset((char *)buf, 0, kBufSize);
  omnifuzz::MutationResult r = omnifuzz::MutationResult::kNoResult;
  
  while (r != omnifuzz::MutationResult::kCycleDone) {
    r = mutator.Mutate(buf, kBufSize);
    for (size_t i = 0; i < kBufSize; ++i) {
      for (size_t  j = 0; j < 8; ++j) {
        printf("%1d", (buf[i] & (128 >> j))? 1: 0);
      }
    }
    printf("\n");
  }


  return 0;
}
