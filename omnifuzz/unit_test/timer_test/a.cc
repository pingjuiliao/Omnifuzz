#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <iostream>


#include "quicksort.h"
#include "omnifuzz/timer.h"
int cmp(const void* a, const void* b) {
  return (*(int *) a - *(int *) b);
}

int
main(int argc, char** argv) {
  uint64_t r0 = 0;
  uint64_t r1 = 0;
  int nums0[] = {1, 17, 2, 9, 8, 9, 20, 1337, 1618, 666, 87};  
  int nums1[] = {1, 17, 2, 9, 8, 9, 20, 1337, 1618, 666, 87};  
  omnifuzz::Timer *t = new omnifuzz::Timer();
  size_t kN = 1000;

  // quicksort
  t->Start();
  quicksort(nums0, 11); 
  r0 = t->Stop();
  
  PrintIntArray(nums0, 11);
  
  // cout
  t->Start();
  qsort(nums1, 11, sizeof(int), cmp);
  r1 = t->Stop();
  PrintIntArray(nums1, 11);
  std::cout << "Prinf:" << r0 << std::endl;
  std::cout << "Cout :" << r1 << std::endl;
  delete t; 

  return 0;
}
