#include <stdio.h>
#include <string.h>

#include <iostream>
#include <cstdint>


#include "omnifuzz/mutator/mutator.h"
#include "omnifuzz/mutator/bit_flip.h"

size_t kBufSize = 4;


void print_word_as_bits(uint32_t val) {
  printf(" ||");
  for (int i = 0; i < 4; i++) {
    for (int j = 7; j >= 0; j--) {
      printf("%1d", ((val >> 8 * i) & (1 << j))? 1: 0);
    }
  }
}


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
    print_word_as_bits(*p);
    print_word_as_bits(k);
    printf("\n");
    k <<= 1;
  }

  return 0;
}
