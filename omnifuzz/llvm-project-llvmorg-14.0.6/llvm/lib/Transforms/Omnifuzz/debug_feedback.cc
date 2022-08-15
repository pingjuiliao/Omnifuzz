#include "llvm/Transforms/Omnifuzz/debug_feedback.h"
#include "llvm/Transforms/Omnifuzz/feedback.h"

namespace omnifuzz {

void DebugFeedback::operatesOnBasicBlock(uint32_t basic_block_id) {
  std::cout << "operated on bbid: " << basic_block_id << std::endl;
}
  
} // namespace omnifuzz
