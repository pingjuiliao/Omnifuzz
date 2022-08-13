add_library(FuzzerFeedback
  afl_feedback.cc
  ${INCLUDE_DIR}/Omnifuzz/Fuzzers/afl_feedback.h
)

target_include_directories(FuzzerFeedback PRIVATE
  ${INCLUDE_DIR})
