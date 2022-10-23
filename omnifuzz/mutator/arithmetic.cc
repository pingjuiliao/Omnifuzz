#include "omnifuzz/mutator/arithmetic.h"

namespace omnifuzz {

ArithmeticMutator::ArithmeticMutator(MutationGranularity g) {
  ptr_start_ = nullptr;
  ptr_end_ = nullptr;
  num_bytes_ = GetGranularitySize(g) >> 3;
  if (num_bytes_ == 0) {
    throw "Cannot construct ArithmeticMutator with weird num_bytes";
  }
}

ArithmeticMutator::~ArithmeticMutator() {}

MutationResult ArithmeticMutator::Mutate(uint8_t* data, size_t& size, 
                                         Testcase* testcase, 
                                         FuzzerState* fuzz_state) {
  MutationResult result;

  switch (num_bytes_) {
    case 1:
      result = MutateAndIterate(data, size);
      break;
    case 2:
      if (size < 2)
        break;
      result = MutateAndIterate(reinterpret_cast<uint16_t*>(data), size);
      break;
    case 4:
      if (size < 4)
        break;
      result = MutateAndIterate(reinterpret_cast<uint32_t*>(data), size);
      break;
    case 8:
      if (size < 8)
        break;
      result = MutateAndIterate(reinterpret_cast<uint64_t*>(data), size);
      break;
    default:
      return MutationResult::kAbort;
  }
  
  return result;
}

// TODO: Support big endian
template <typename T>
MutationResult ArithmeticMutator::MutateAndIterate(T* buf, size_t size) {
  static T original;
  static T num = 1;
  static bool positive = true;
  T* window;
  
  // mutating new testcase
  if (!ptr_start_) {
    ptr_start_ = reinterpret_cast<uint8_t*>(buf);
    ptr_end_ = ptr_start_ + size;
    window = buf;
    original = *window;
  } else {
    window = reinterpret_cast<T*>(ptr_start_);
  }

  // move index if it reaches kArithmeticMax 
  if (num > kArithmeticMax) { 
    *window = original; 
    ptr_start_ ++;
    window = reinterpret_cast<T*>(ptr_start_);
    
    // initialize for the new index
    if (((uint8_t *) window) + sizeof(T) <= ptr_end_) {
      original = *window;
    }
    num = 1;
    positive = true;
  }
  
  if (ptr_start_ + num_bytes_ > ptr_end_) {
    ptr_start_ = nullptr;
    ptr_end_ = nullptr;
    return MutationResult::kCycleDone;
  }

  // mutate x+1, x-1, x+2, x-2, x+3, x-3, ..., x+35, x-35
  if (positive) {
    *window = original + num;
    positive = false;
  } else {
    *window = original - num;
    positive = true;
    num++;
  }
  return MutationResult::kSuccess;
}

MutationResult ArithmeticMutator::RandomMutate(uint8_t* data, size_t& size) {
  static bool recover_mode = false;
  static uint32_t position = 0;
  static bool positive = false;

  MutationResult result = MutationResult::kCycleDone;
  if (!recover_mode) {
    position = rand() % (size - num_bytes_ + 1);
    positive = (rand() % 2 == 0);
    result = MutationResult::kSuccess;
  }
  positive = !positive;
  recover_mode = !recover_mode;

  switch (num_bytes_) {
    case 1:
      MutateOne(reinterpret_cast<uint8_t*>(&data[position]), positive);
      break;
    case 2:
      MutateOne(reinterpret_cast<uint16_t*>(&data[position]), positive);
      break;
    case 4:
      MutateOne(reinterpret_cast<uint32_t*>(&data[position]), positive);
      break;
    case 8:
      MutateOne(reinterpret_cast<uint64_t*>(&data[position]), positive);
      break;
    default:
      break;
  }
  return result;
}

template <typename T>
void ArithmeticMutator::MutateOne(T* data, bool is_positive) {
  T operand = kArithmeticMax; 
  if (is_positive) {
    *data = *data + operand;
  } else {
    *data = *data - operand;
  }
}

} // namespace omnifuzz
