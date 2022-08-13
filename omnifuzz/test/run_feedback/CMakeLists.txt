add_executable(run_feedback
  main.cc
  ${INCLUDE_DIR}/Omnifuzz/Fuzzers/afl_feedback.h
)


target_include_directories(run_feedback PUBLIC ${INCLUDE_DIR})
target_link_libraries(run_feedback
  PUBLIC FuzzerFeedback)
