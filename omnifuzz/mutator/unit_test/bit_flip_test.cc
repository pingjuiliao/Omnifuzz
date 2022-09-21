#include <stdio.h>
#include <string.h>

#include <iostream>
#include <cstdint>


#include "../mutator.h"
#include "../bit_flip.h"

const size_t kBufSize = 4;

int main(int argc, char** argv) {
  uint8_t buf[kBufSize];
  omnifuzz::BitFlipMutator mutator(omnifuzz::MutationGranularity::kBit);
  memset((char *)buf, 0, kBufSize);
  uint32_t k = 1;
  omnifuzz::MutationResult r = omnifuzz::MutationResult::kNumResults; 
  while (r != omnifuzz::MutationResult::kCycleDone) {
    r = mutator.Mutate(buf, kBufSize);
    for (size_t i = 0; i < kBufSize; ++i) {
      for (size_t j = 0; j < 8; ++j) {
        printf("%1d", (buf[i] & (128 >> j))? 1: 0);
      }    
    } 
    uint32_t *p = (uint32_t *)buf;
    printf(" || %032b || %032b\n", *p, k);
    k <<= 1;
  }

  return 0;
}
