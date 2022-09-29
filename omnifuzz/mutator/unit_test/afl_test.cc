#include <stdio.h>
#include <string.h>

#include <iostream>
#include <cstdint>


#include "../afl_mutator.h"
// An odd number is more likely to break things.
size_t kBufSize = 11;
const size_t kNumBuffer = 2;
const uint8_t buf_memset[kNumBuffer] = {0x00, 0xff};
int main(int argc, char** argv) {
  uint8_t *buf = (uint8_t *) malloc(kBufSize);
  omnifuzz::AflMutator mutator;

  for (int n = 0; n < kNumBuffer; ++n) {
    puts("###########################");
    printf("BUFFER %s\n", (char *) buf);
    puts("###########################");
    
    memset(buf, buf_memset[n], kBufSize);
    omnifuzz::MutationResult r = omnifuzz::MutationResult::kNumResults; 
    while (r != omnifuzz::MutationResult::kCycleDone) {
      r = mutator.Mutate(buf, kBufSize);
      for (size_t i = 0; i < kBufSize; ++i) {
        printf("%02x ", buf[i]);
      }
      printf(" | ");
      for (size_t i = 0; i < kBufSize; ++i) {
        for (size_t j = 0; j < 8; ++j) {
          printf("%1d", (buf[i] & (128 >> j))? 1: 0);
        }    
      }
      printf("\n");
    }
  }
  return 0;
}
