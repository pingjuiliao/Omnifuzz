#include "arithmetic.h"

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

MutationResult ArithmeticMutator::Mutate(uint8_t* data, size_t len) {
  MutationResult result;
  if (!ptr_start_) {
    ptr_start_ = data;
    ptr_end_ = data + len;
  }

  switch (num_bytes_) {
    case 1:
      result = MutateAndIterate(data);
      break;
    case 2:
      result = MutateAndIterate(reinterpret_cast<uint16_t*>(data));
      break;
    case 4:
      result = MutateAndIterate(reinterpret_cast<uint32_t*>(data));
      break;
    case 8:
      result = MutateAndIterate(reinterpret_cast<uint64_t*>(data));
      break;
    default:
      return MutationResult::kAbort;
  }
  
  return result;
}

// TODO: BIG ENDIAN
template <typename T>
MutationResult ArithmeticMutator::MutateAndIterate(T* buf) {
  static T original = *buf;
  static T num = 1;
  static bool positive = true;
  T* window = reinterpret_cast<T*>(ptr_start_);

  // move index if it reaches kArithmeticMax 
  if (num > kArithmeticMax) { 
    *window = original; 
    ptr_start_ ++;
    window = reinterpret_cast<T*>(ptr_start_);
    
    // initialize for the new index
    original = *window;
    num = 1;
    positive = true;
  }
  
  if (ptr_start_ + num_bytes_ > ptr_end_)
    return MutationResult::kCycleDone;

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

} // namespace omnifuzz
