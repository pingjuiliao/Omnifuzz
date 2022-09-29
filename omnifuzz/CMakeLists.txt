add_subdirectory(executor)
add_subdirectory(scheduler)
add_subdirectory(mutator)
add_subdirectory(feedback)

set(OMNIFUZZ_LIB_TYPE STATIC)

# testcase_file_manager
add_library(omnifuzz_testcase_file_manager ${OMNIFUZZ_LIB_TYPE}
  testcase_file_manager.cc
  testcase_file_manager.h
)
add_library(omnifuzz::testcase_file_manager ALIAS
  omnifuzz_testcase_file_manager
)
target_include_directories(omnifuzz_testcase_file_manager
  PUBLIC ${OMNIFUZZ_INCLUDE_DIR}
)

# fuzzer
add_library(omnifuzz_fuzzer ${OMNIFUZZ_LIB_TYPE}
  fuzzer.cc
  fuzzer.h
)

add_library(omnifuzz::fuzzer ALIAS
  omnifuzz_fuzzer
)

target_link_libraries(omnifuzz_fuzzer
  PUBLIC omnifuzz::testcase_file_manager
         omnifuzz::executor
         omnifuzz::mutator
)
target_include_directories(omnifuzz_fuzzer
  PUBLIC ${OMNIFUZZ_INCLUDE_DIR}
  )

