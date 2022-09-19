CXX=g++

all: afl_test
afl_test: afl_test.cc interesting arithmetic bit_flip mutator afl_mutator
	$(CXX) -c $< -o $@.o
	$(CXX) $@.o afl_mutator.o interesting.o mutator.o bit_flip.o arithmetic.o -o $@.exe
interest_test: interest_test.cc interesting mutator
	$(CXX) -c $< -o $@.o
	$(CXX) $@.o interesting.o mutator.o -o $@.exe
arith_test: arith_test.cc arithmetic mutator
	$(CXX) -c $< -o $@.o
	$(CXX) $@.o arithmetic.o mutator.o -o $@.exe
composite_test: composite_test.cc bit_flip mutator
	$(CXX) -c $< -o $@.o
	$(CXX) $@.o bit_flip.o mutator.o -o $@.exe
bit_flip_test: bit_flip_test.cc bit_flip mutator
	$(CXX) -c $< -o $@.o
	$(CXX) $@.o bit_flip.o mutator.o -o $@.exe
afl_mutator: ../afl_mutator.cc
	$(CXX) -c $< -o $@.o
interesting: ../interesting.cc
	$(CXX) -c $< -o $@.o
arithmetic: ../arithmetic.cc
	$(CXX) -c $< -o $@.o 
bit_flip: ../bit_flip.cc
	$(CXX) -c $< -o $@.o
mutator: ../mutator.cc
	$(CXX) -c $< -o $@.o
clean:
	rm $(wildcard *.o) $(wildcard *.exe)
