cmake_minimum_required(VERSION 3.11)


project("Omnifuzz")
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

# add_subdirectory(src/FeedbackSystem)
add_subdirectory(src/Fuzzers)
add_subdirectory(test/run_feedback)

