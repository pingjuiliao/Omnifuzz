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
  int nums0[] = {1, 17, 2, 9, 8, 9, 20, 1337, 1618, 666, 87};  
  int nums1[] = {1, 17, 2, 9, 8, 9, 20, 1337, 1618, 666, 87};  
  
  omnifuzz::Timer t;
  size_t kN = 1000;

  // t.Start();
  quicksort(nums0, 11); 
  // uint64_t r = t.Stop();
  
  PrintIntArray(nums0, 11);
  
  // t.Start();
  qsort(nums1, 11, sizeof(int), cmp);
  // uint64_t r1 = t.Stop();
  PrintIntArray(nums1, 11);
  // std::cout << "my quicksort: " << r << std::endl;
  // std::cout << "stdlib qsort: " << r1 << std::endl;
  

  return 0;
}
