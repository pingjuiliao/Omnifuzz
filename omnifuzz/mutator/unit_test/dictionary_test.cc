#include <stdio.h>
#include <string.h>

#include <iostream>
#include <cstdint>

#include "omnifuzz/mutator/mutator.h"
#include "omnifuzz/mutator/dictionary.h"

size_t kBufSize = 20;


int main(int argc, char** argv) {
  uint8_t buf[kBufSize+1];
  buf[kBufSize] = '\0';

  std::vector<std::string> seqs;
  seqs.push_back("hello");
  seqs.push_back("Raymond Holt");
  seqs.push_back("qiuqiu");

  omnifuzz::DictionaryMutator mutator(seqs);
  memset((char *)buf, '0', kBufSize);
  
  omnifuzz::MutationResult r = omnifuzz::MutationResult::kNumResults; 
  while (r != omnifuzz::MutationResult::kCycleDone) {
    r = mutator.Mutate(buf, kBufSize);
    std::cout << buf << std::endl;
  }

  return 0;
}
