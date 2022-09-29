cmake_minimum_required(VERSION 3.11)

project("Omnifuzz")


## Compiler
set(CMAKE_CXX_STANDARD 17)

## LLVM settings
set(LLVM_ENABLE_PROJECTS "clang;" CACHE STRING "enable clang" FORCE)

set(LLVM_BUILD_TOOLS ON  CACHE BOOL "enable tools such as opt")
set(LLVM_BUILD_TESTS OFF CACHE BOOL "disable tests")
set(LLVM_BUILD_DOCS  OFF CACHE BOOL "disable docs")
set(LLVM_BUILD_EXMAPLES OFF CACHE BOOL "disable build examples")
set(LLVM_INCLUDE_EXAMPLES OFF CACHE BOOL "disable examples")
set(LLVM_ENABLE_LTO OFF CACHE BOOL "disable link-time optimizatio")
set(LLVM_ENABLE_DOXYGEN OFF CACHE BOOL "disable doxygen")
set(LLVM_ENBALE_RTTI OFF CACHE BOOL "disable rtti?????")
set(CMAKE_BUILD_TYPE Release)
set(BUILD_SHARED_LIBS OFF)


## LLVM needs to know about Omnifuzz
set(OMNIFUZZ_LIB_TYPE STATIC)
set(OMNIFUZZ_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/")
set(OMNIFUZZ_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/")
message("omnifuzz_main_include: ${OMNIFUZZ_INCLUDE_DIR}")
message("I want to include: ${OMNIFUZZ_INCLUDE_DIR}/Omnifuzz/FeedbackSystem/debug_feedback.h")
add_subdirectory(llvm-project/llvm)
add_subdirectory(omnifuzz)
add_subdirectory(fuzzers)
