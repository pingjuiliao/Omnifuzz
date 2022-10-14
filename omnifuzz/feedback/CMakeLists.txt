# mechanism
add_library(omnifuzz_mechanism ${OMNIFUZZ_LIB_TYPE}
  mechanism.cc
  mechanism.h)

add_library(omnifuzz::mechanism ALIAS
  omnifuzz_mechanism)
target_include_directories(omnifuzz_mechanism
  PUBLIC ${OMNIFUZZ_INCLUDE_DIR})

# afl_mechanism
add_library(omnifuzz_afl_mechanism ${OMNIFUZZ_LIB_TYPE}
  afl_mechanism.cc
  afl_mechanism.h
)
add_library(omnifuzz::afl_mechanism ALIAS
  omnifuzz_afl_mechanism)
target_include_directories(omnifuzz_afl_mechanism
  PUBLIC ${OMNIFUZZ_INCLUDE_DIR})


# debug_mechanism
add_library(omnifuzz_debug_mechanism ${OMNIFUZZ_LIB_TYPE}
  debug_mechanism.cc
  debug_mechanism.h
)

add_library(omnifuzz::debug_mechanism ALIAS
  omnifuzz_debug_mechanism)
target_include_directories(omnifuzz_debug_mechanism
  PUBLIC ${OMNIFUZZ_INCLUDE_DIR})
