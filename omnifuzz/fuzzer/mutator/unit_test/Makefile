CXX=g++

all: arith_test bit_flip_test composite_test
arith_test: arith_test.cc arithmetic mutator
	$(CXX) -c $< -o $@.o
	$(CXX) $@.o arithmetic.o mutator.o -o $@.exe
composite_test: composite_test.cc bit_flip mutator
	$(CXX) -c $< -o $@.o
	$(CXX) $@.o bit_flip.o mutator.o -o $@.exe
bit_flip_test: bit_flip_test.cc bit_flip mutator
	$(CXX) -c $< -o $@.o
	$(CXX) $@.o bit_flip.o mutator.o -o $@.exe
arithmetic: ../arithmetic.cc
	$(CXX) -c $< -o $@.o 
bit_flip: ../bit_flip.cc
	$(CXX) -c $< -o $@.o
mutator: ../mutator.cc
	$(CXX) -c $< -o $@.o
clean:
	rm $(wildcard *.o) $(wildcard *.exe)
