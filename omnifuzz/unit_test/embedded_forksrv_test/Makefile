
ROOT=../../../
CXX=g++

## Instrumentation
CLANG=$(ROOT)/build/llvm-project/llvm/bin/clang
CLANGXX=$(ROOT)/build/llvm-project/llvm/bin/clang++
OPT=$(ROOT)/build/llvm-project/llvm/bin/opt

CXXFLAGS=-I$(ROOT) -std=c++17


all: fsrv_exec_test embedded
embedded: embedded.c
	$(CLANG) -o $@.ll -O1 -m64 -emit-llvm -S $<
	$(OPT) -o $@.bc -passes=omnifuzz-debug $@.ll
	$(CLANG) -o $@.exe $@.bc
fsrv_exec_test: fsrv_exec_test.cc forksrv_executor fork_client forkserver executor debug_feedback
	$(CXX) $(CXXFLAGS) -o $@.o -c $<
	$(CXX) $@.o forksrv_executor.o fork_client.o forkserver.o executor.o debug_feedback.o -o $@.exe
forksrv_executor: $(ROOT)/omnifuzz/executor/forksrv_executor.cc
	$(CXX) $(CXXFLAGS) -DUNIT_TEST -o $@.o -c $<
fork_client: $(ROOT)/omnifuzz/executor/fork_client.cc
	$(CXX) $(CXXFLAGS) -DUNIT_TEST -o $@.o -c $<
forkserver: $(ROOT)/omnifuzz/executor/forkserver.cc
	$(CXX) $(CXXFLAGS) -DUNIT_TEST -o $@.o -c $<
executor: $(ROOT)/omnifuzz/executor/executor.cc
	$(CXX) $(CXXFLAGS) -DUNIT_TEST -o $@.o -c $<
debug_feedback: $(ROOT)/omnifuzz/feedback/debug_feedback_mechanism.cc
	$(CXX) $(CXXFLAGS) -DUNIT_TEST -o $@.o -c $<
clean:
	rm $(wildcard *.o) $(wildcard *.exe)
