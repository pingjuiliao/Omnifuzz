# Grey-box
add_executable(grey_box
  grey_box.cc
)

target_link_libraries(grey_box
  PUBLIC omnifuzz::fuzzer
         omnifuzz::forksrv_executor
         omnifuzz::list_scheduler
         omnifuzz::deterministic_mutator
         omnifuzz::afl_mechanism
)
target_include_directories(grey_box
  PUBLIC ${OMNIFUZZ_INCLUDE_DIR})

# AFL
add_executable(afl
  afl.cc
)
target_link_libraries(afl
  PUBLIC omnifuzz::fuzzer
         omnifuzz::forksrv_executor
         omnifuzz::afl_scheduler
         omnifuzz::afl_mutator
         omnifuzz::afl_mechanism
)
target_include_directories(afl
  PUBLIC ${OMNIFUZZ_INCLUDE_DIR})

# copy the program under test
file(COPY "program_under_test/"
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/program_under_test")

