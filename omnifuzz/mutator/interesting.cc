#include "omnifuzz/mutator/interesting.h"

namespace omnifuzz {

template <typename T>
T InterestingInt::GetLittleEndian(void) {
  switch (sizeof(T)) {
    case 1:
      if (number_ < -128 || 127 < number_)
        throw "Trying to convert invalid number to int8_t";
      break;
    case 2:
      if (number_ < -32768 || 32767 < number_)
        throw "Trying to convert invalid number to int16_t";
      break;
    case 4:
      if (number_ < -2147483648 || 2147483647 < number_) 
        throw "Trying to convert invalid number to int32_t";
      break;
    default:
      throw "Interesting Int not supported";
  }
  int64_t* ptr = &number_;
  T* reinterpreted_ptr = reinterpret_cast<T*>(ptr);
  T little_endian_ = *reinterpreted_ptr;
  return little_endian_;
}


InterestingInt8Mutator::InterestingInt8Mutator() {
  // Initial state: 
  // avoid state management because composite needs to broadcast them
  ptr_start_ = nullptr;
  ptr_end_ = nullptr;
  interesting_values_.push_back(new InterestingInt(-128));
  interesting_values_.push_back(new InterestingInt(-1));
  interesting_values_.push_back(new InterestingInt(0));
  interesting_values_.push_back(new InterestingInt(1));
  interesting_values_.push_back(new InterestingInt(16));
  interesting_values_.push_back(new InterestingInt(32));
  interesting_values_.push_back(new InterestingInt(64));
  interesting_values_.push_back(new InterestingInt(100));
  interesting_values_.push_back(new InterestingInt(127));
  iterator_ = interesting_values_.begin();
}

InterestingInt8Mutator::~InterestingInt8Mutator() {
  interesting_values_.clear();
}

MutationResult InterestingInt8Mutator::Mutate(uint8_t* data, size_t len) {
  
  if (!ptr_start_) {
    ptr_start_ = data;
    ptr_end_ = data + len;
    original_ = *ptr_start_;
  }

  if (iterator_ == interesting_values_.end()) {
    *ptr_start_ = original_;
    ptr_start_++;

    // initialize for the new index 
    original_ = *ptr_start_;
    iterator_ = interesting_values_.begin();
  }

  if (ptr_start_ >= ptr_end_) {
    ptr_start_ = nullptr;
    ptr_end_ = nullptr;
    return MutationResult::kCycleDone;
  }

  // mutate
  *ptr_start_ = (*iterator_)->GetLittleEndian<uint8_t>();
  iterator_++;
 
  return MutationResult::kSuccess;
}

InterestingInt16Mutator::InterestingInt16Mutator() {
  ptr_start_ = nullptr;
  ptr_end_ = nullptr;
  interesting_values_.push_back(new InterestingInt(-32768));
  interesting_values_.push_back(new InterestingInt(-129));
  interesting_values_.push_back(new InterestingInt(128));
  interesting_values_.push_back(new InterestingInt(255));
  interesting_values_.push_back(new InterestingInt(256));
  interesting_values_.push_back(new InterestingInt(512));
  interesting_values_.push_back(new InterestingInt(1000));
  interesting_values_.push_back(new InterestingInt(1024));
  interesting_values_.push_back(new InterestingInt(4096));
  interesting_values_.push_back(new InterestingInt(32767));
  iterator_ = interesting_values_.begin();
}

MutationResult InterestingInt16Mutator::Mutate(uint8_t* data, size_t len) {
  
  uint16_t* window;
  if (!ptr_start_) {
    ptr_start_ = data;
    ptr_end_ = data + len;
    window = reinterpret_cast<uint16_t*>(ptr_start_);
    original_ = *window;
  } else {
    window = reinterpret_cast<uint16_t*>(ptr_start_);
  }


  if (iterator_ == interesting_values_.end()) {
    *window = original_;
    ptr_start_++;
    window = reinterpret_cast<uint16_t*>(ptr_start_);

    // initialize for the new index 
    original_ = *window;
    iterator_ = interesting_values_.begin();
  }

  if (ptr_start_ + kNumBytes > ptr_end_) {
    ptr_start_ = nullptr;
    ptr_end_ = nullptr;
    return MutationResult::kCycleDone;
  }

  // mutate
  *window = (*iterator_)->GetLittleEndian<uint16_t>();
  iterator_++;
 
  return MutationResult::kSuccess;
}

InterestingInt32Mutator::InterestingInt32Mutator() {
  ptr_start_ = nullptr;
  ptr_end_ = nullptr;
  interesting_values_.push_back(new InterestingInt(-2147483648LL));
  interesting_values_.push_back(new InterestingInt(-100663046));
  interesting_values_.push_back(new InterestingInt(-32769));
  interesting_values_.push_back(new InterestingInt(32768));
  interesting_values_.push_back(new InterestingInt(65535));
  interesting_values_.push_back(new InterestingInt(65536));
  interesting_values_.push_back(new InterestingInt(100663045));
  interesting_values_.push_back(new InterestingInt(2147483647));
  iterator_ = interesting_values_.begin();
}

MutationResult InterestingInt32Mutator::Mutate(uint8_t* data, size_t len) {
  uint32_t* window;
  if (!ptr_start_) {
    ptr_start_ = data;
    ptr_end_ = data + len;
    window = reinterpret_cast<uint32_t*>(ptr_start_);
    original_ = *window;
  } else {
    window = reinterpret_cast<uint32_t*>(ptr_start_);
  }


  if (iterator_ == interesting_values_.end()) {
    *window = original_;
    ptr_start_++;
    window = reinterpret_cast<uint32_t*>(ptr_start_);

    // initialize for the new index 
    original_ = *window;
    iterator_ = interesting_values_.begin();
  }

  if (ptr_start_ + kNumBytes > ptr_end_) {
    ptr_start_ = nullptr;
    ptr_end_ = nullptr;
    return MutationResult::kCycleDone;
  }

  // mutate
  *window = (*iterator_)->GetLittleEndian<uint32_t>();
  iterator_++;
 
  return MutationResult::kSuccess;
}


} // namespace omnifuzz
