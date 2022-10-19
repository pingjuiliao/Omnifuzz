# list_scheduler
add_library(omnifuzz_list_scheduler ${OMNIFUZZ_LIB_TYPE}
  list_scheduler.cc
  list_scheduler.h
)

add_library(omnifuzz::list_scheduler ALIAS
  omnifuzz_list_scheduler)
target_include_directories(omnifuzz_list_scheduler
  PUBLIC ${OMNIFUZZ_INCLUDE_DIR}
)


# afl_scheduler
add_library(omnifuzz_afl_scheduler ${OMNIFUZZ_LIB_TYPE}
  afl_scheduler.cc
  afl_scheduler.h)

add_library(omnifuzz::afl_scheduler ALIAS
  omnifuzz_afl_scheduler)
target_include_directories(omnifuzz_afl_scheduler
  PUBLIC ${OMNIFUZZ_INCLUDE_DIR})


