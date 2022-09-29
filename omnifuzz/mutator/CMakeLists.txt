
# bit flip
add_library(omnifuzz_bit_flip ${OMNIFUZZ_LIB_TYPE}
  bit_flip.cc
  bit_flip.h
)

add_library(omnifuzz::bit_flip ALIAS
  omnifuzz_bit_flip)
target_include_directories(omnifuzz_bit_flip
  PUBLIC ${OMNIFUZZ_INCLUDE_DIR})

# arithmetic
add_library(omnifuzz_arithmetic ${OMNIFUZZ_LIB_TYPE}
  arithmetic.cc
  arithmetic.h
)

add_library(omnifuzz::arithmetic ALIAS
  omnifuzz_arithmetic)
target_include_directories(omnifuzz_arithmetic
  PUBLIC ${OMNIFUZZ_INCLUDE_DIR})

# interesting
add_library(omnifuzz_interesting ${OMNIFUZZ_LIB_TYPE}
  interesting.cc
  interesting.h
)
add_library(omnifuzz::interesting ALIAS
  omnifuzz_interesting)
target_include_directories(omnifuzz_interesting
  PUBLIC ${OMNIFUZZ_INCLUDE_DIR})

# mutator
add_library(omnifuzz_mutator ${OMNIFUZZ_LIB_TYPE}
  mutator.cc
  mutator.h
)

add_library(omnifuzz::mutator ALIAS
  omnifuzz_mutator)
target_include_directories(omnifuzz_mutator
  PUBLIC ${OMNIFUZZ_INCLUDE_DIR})

# afl_mutator
add_library(omnifuzz_afl_mutator ${OMNIFUZZ_LIB_TYPE}
  afl_mutator.cc
  afl_mutator.h
)

add_library(omnifuzz::afl_mutator ALIAS
  omnifuzz_afl_mutator)

target_link_libraries(omnifuzz_afl_mutator
  PUBLIC omnifuzz::mutator
         omnifuzz::bit_flip
         omnifuzz::arithmetic
         omnifuzz::interesting
)
target_include_directories(omnifuzz_afl_mutator
  PUBLIC ${OMNIFUZZ_INCLUDE_DIR})
