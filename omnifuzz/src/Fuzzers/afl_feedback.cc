#include "Omnifuzz/Fuzzers/afl_feedback.h"

using namespace omnifuzz;

AFLFeedback::AFLFeedback() {}

AFLFeedback::~AFLFeedback() {}

void AFLFeedback::operationOnBasicBlock(uint32_t basic_block_id) { 
  std::cout << "afl bbid is: " << basic_block_id << std::endl;
  uint32_t edge = basic_block_id ^ prev_basic_block_id;
  edge %= BITMAP_SIZE;
  bitmap[edge] ++;
  prev_basic_block_id = basic_block_id >> 1;
}

