CXX=g++
all: fsrv_instance fsrv_exec_test
run: fsrv_exec_test.exe fsrv_instance.exe
	./$<
fsrv_exec_test: fsrv_exec_test.cc forksrv_executor fork_client forkserver executor
	$(CXX) -o $@.o -c $<
	$(CXX) $@.o forksrv_executor.o fork_client.o forkserver.o executor.o -o $@.exe
fsrv_instance: fsrv_instance.cc forkserver 
	$(CXX) -o $@.o -c $<
	$(CXX) $@.o forkserver.o -o $@.exe
forksrv_executor: ../forksrv_executor.cc
	$(CXX) -DUNIT_TEST -o $@.o -c $<
fork_client: ../fork_client.cc 
	$(CXX) -DUNIT_TEST -o $@.o -c $<
forkserver: ../forkserver.cc
	$(CXX) -DUNIT_TEST -o $@.o -c $<
executor: ../executor.cc
	$(CXX) -DUNIT_TEST -o $@.o -c $<
clean:
	rm $(wildcard *.o) $(wildcard *.exe)
