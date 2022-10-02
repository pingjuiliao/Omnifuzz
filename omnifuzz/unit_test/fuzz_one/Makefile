ROOT=../../..
CC=gcc
CXX=g++

## Instrumentation
CLANG=$(ROOT)/build/llvm-project/llvm/bin/clang
CLANGXX=$(ROOT)/build/llvm-project/llvm/bin/clang++
OPT=$(ROOT)/build/llvm-project/llvm/bin/opt

CXXFLAGS=-I$(ROOT) -std=c++17

all: fuzz_one a
fuzz_one: fuzz_one.cc forksrv_executor fork_client forkserver executor afl_feedback list_scheduler afl_mutator bit_flip mutator arithmetic interesting tfm
	$(CXX) $(CXXFLAGS) -DUNIT_TEST -o $@.o -c $<
	$(CXX) $@.o forksrv_executor.o fork_client.o forkserver.o executor.o afl_feedback.o list_scheduler.o afl_mutator.o bit_flip.o mutator.o arithmetic.o interesting.o tfm.o -o $@.exe
a: a.c
	$(CLANG) -o $@.ll -O0 -m64 -emit-llvm -S $<
	$(OPT) -o $@.bc -passes=omnifuzz-afl $@.ll
	$(CLANG) -O0 -o $@.exe $@.bc
tfm: $(ROOT)/omnifuzz/testcase_file_manager.cc
	$(CXX) $(CXXFLAGS) -DUNIT_TEST -o $@.o -c $<
list_scheduler: $(ROOT)/omnifuzz/scheduler/list_scheduler.cc
	$(CXX) $(CXXFLAGS) -DUNIT_TEST -o $@.o -c $<
afl_mutator: $(ROOT)/omnifuzz/mutator/afl_mutator.cc	
	$(CXX) $(CXXFLAGS) -DUNIT_TEST -o $@.o -c $<
interesting: $(ROOT)/omnifuzz/mutator/interesting.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@.o
arithmetic: $(ROOT)/omnifuzz/mutator/arithmetic.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@.o 
bit_flip: $(ROOT)/omnifuzz/mutator/bit_flip.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@.o
mutator: $(ROOT)/omnifuzz/mutator/mutator.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@.o
forksrv_executor: $(ROOT)/omnifuzz/executor/forksrv_executor.cc
	$(CXX) $(CXXFLAGS) -DUNIT_TEST -o $@.o -c $<
fork_client: $(ROOT)/omnifuzz/executor/fork_client.cc
	$(CXX) $(CXXFLAGS) -DUNIT_TEST -o $@.o -c $<
forkserver: $(ROOT)/omnifuzz/executor/forkserver.cc
	$(CXX) $(CXXFLAGS) -DUNIT_TEST -o $@.o -c $<
executor: $(ROOT)/omnifuzz/executor/executor.cc
	$(CXX) $(CXXFLAGS) -DUNIT_TEST -o $@.o -c $<
afl_feedback: $(ROOT)/omnifuzz/feedback/afl_feedback_mechanism.cc
	$(CXX) $(CXXFLAGS) -DUNIT_TEST -o $@.o -c $<
clean:
	rm $(wildcard *.o) $(wildcard *.exe) $(wildcard *.bc) $(wildcard *.ll) 
