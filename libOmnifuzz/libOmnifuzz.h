#ifndef LIB_OMNIFUZZ_H
#define LIB_OMNIFUZZ_H
#include <iostream>
#include <cstdint>

#define MAPSIZE 64 * 1024

uint8_t bitmap[MAPSIZE];
uint32_t prev_random;

extern "C" void _omnifuzz_basicblock(uint32_t);


#endif // LIB_OMNIFUZZ_H
