# feedback_mechanism
add_library(omnifuzz_feedback_mechanism ${OMNIFUZZ_LIB_TYPE}
  feedback_mechanism.cc
  feedback_mechanism.h)

add_library(omnifuzz::feedback_mechanism ALIAS
  omnifuzz_feedback_mechanism)
target_include_directories(omnifuzz_feedback_mechanism
  PUBLIC ${OMNIFUZZ_INCLUDE_DIR})

# afl_feedback_mechanism
add_library(omnifuzz_afl_feedback_mechanism ${OMNIFUZZ_LIB_TYPE}
  afl_feedback_mechanism.cc
  afl_feedback_mechanism.h
)
add_library(omnifuzz::afl_feedback_mechanism ALIAS
  omnifuzz_afl_feedback_mechanism)
target_include_directories(omnifuzz_afl_feedback_mechanism
  PUBLIC ${OMNIFUZZ_INCLUDE_DIR})


# debug_feedback_mechanism
add_library(omnifuzz_debug_feedback_mechanism ${OMNIFUZZ_LIB_TYPE}
  debug_feedback_mechanism.cc
  debug_feedback_mechanism.h
)

add_library(omnifuzz::debug_feedback_mechanism ALIAS
  omnifuzz_debug_feedback_mechanism)
target_include_directories(omnifuzz_debug_feedback_mechanism
  PUBLIC ${OMNIFUZZ_INCLUDE_DIR})
