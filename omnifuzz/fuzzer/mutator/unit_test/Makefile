CXX=g++

all: bit_flip_test composite_test
composite_test: composite_test.cc bit_flip mutator
	$(CXX) -c $< -o $@.o
	$(CXX) $@.o bit_flip.o mutator.o -o $@.exe
bit_flip_test: bit_flip_test.cc bit_flip mutator
	$(CXX) -c $< -o $@.o
	$(CXX) $@.o bit_flip.o mutator.o -o $@.exe
bit_flip: ../bit_flip.cc
	$(CXX) -c $< -o $@.o
mutator: ../mutator.cc
	$(CXX) -c $< -o $@.o


