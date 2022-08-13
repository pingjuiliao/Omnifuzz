#include "libOmnifuzz.h"


extern "C" {

  void _omnifuzz_basicblock(uint32_t compile_time_random) {
    std::cout << compile_time_random << std::endl;
    compile_time_random %= MAPSIZE;
    bitmap[compile_time_random ^ prev_random] ++;
    prev_random = compile_time_random;
  }
}
