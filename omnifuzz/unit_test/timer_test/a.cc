#include <cstdio>
#include <cstdint>
#include <iostream>

#include "omnifuzz/timer.h"

int
main(int argc, char** argv) {
  
  omnifuzz::Timer *t = new omnifuzz::Timer();
  size_t kN = 1000;


  // printf
  t->Start();
  for (size_t i = 0; i < kN; ++i) {
    printf("%ld\n", i);
  }
  uint64_t r = t->Stop();
  std::cout << "Prinf:" << r << std::endl;

  // cout
  t->Start();
  for (size_t i = 0; i < kN; ++i) {
    std::cout << i << std::endl; 
  }
  uint64_t r1 = t->Stop();
  std::cout << "Cout :" << r1 << std::endl;

  return 0;
}
