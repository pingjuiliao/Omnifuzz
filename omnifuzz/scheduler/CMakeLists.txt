
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
