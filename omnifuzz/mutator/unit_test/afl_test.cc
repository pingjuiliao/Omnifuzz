#include <stdio.h>
#include <string.h>

#include <iostream>
#include <cstdint>

#include "omnifuzz/mutator/afl_mutator.h"

// An odd number is more likely to break things.
size_t kBufSize = 11;
const size_t kNumBuffer = 2;
const uint8_t buf_memset[kNumBuffer] = {0x00, 0xff};
omnifuzz::Testcase testcases[kNumBuffer];

void Init(void) {
  memset(testcases, 0, sizeof(testcases));
  for (int i = 0; i < kNumBuffer; ++i) {
    testcases[i].file_name = strdup("dummy");
  }
}


int main(int argc, char** argv) {
  uint8_t *buf = (uint8_t *) malloc(kBufSize+1);
  Init();
  omnifuzz::AFLMutator mutator;

  for (int cycle = 0; cycle < 3; ++cycle) {
    int num_mutated = 0;
    for (int n = 0; n < kNumBuffer; ++n) {
      memset(buf, 0, sizeof(kBufSize)-1);
      size_t curr_size = kBufSize;
      memset(buf, buf_memset[n], kBufSize);
      omnifuzz::MutationResult r = omnifuzz::MutationResult::kNumResults; 
      while (r != omnifuzz::MutationResult::kCycleDone) {
        r = mutator.Mutate(buf, curr_size, &testcases[n]);
        for (size_t i = 0; i < kBufSize; ++i) {
          printf("%02hhx ", buf[i]);
        }
        printf(" | ");
        for (size_t i = 0; i < kBufSize; ++i) {
          for (size_t j = 0; j < 8; ++j) {
            printf("%1d", (buf[i] & (128 >> j))? 1: 0);
          }    
        }
        printf("\n");
        num_mutated ++;
      }
    }
    puts("==========================");
    printf("====== Num Mutated: %d\n", num_mutated);
    puts("==========================");
  }
  free(buf);
  return 0;
}
