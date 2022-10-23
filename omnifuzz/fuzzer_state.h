// We allow the state of fuzzer be represented as a hashmap since different 
// fuzzers tend to define their own fuzzing cycle with differnt states.

#ifndef OMNIFUZZ_FUZZER_STATE_H
#define OMNIFUZZ_FUZZER_STATE_H

#include <string>
#include <unordered_map>
#include <utility>

using FuzzerState = std::unordered_map<std::string, std::pair<void*, size_t>>;

#endif  // OMNIFUZZ_FUZZER_STATE_H
