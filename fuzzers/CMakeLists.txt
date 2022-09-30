# AFL
add_executable(afl
  afl.cc
)
target_link_libraries(afl
  PUBLIC omnifuzz::fuzzer
         omnifuzz::forksrv_executor
         omnifuzz::list_scheduler
         omnifuzz::afl_mutator
         omnifuzz::afl_feedback_mechanism
)
target_include_directories(afl
  PUBLIC ${OMNIFUZZ_INCLUDE_DIR})

# copy the program under test
file(COPY "program_under_test/"
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/program_under_test")
