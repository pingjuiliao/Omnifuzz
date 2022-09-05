CXX=g++

all: main 
main: main.cc instrumentator
	$(CXX) -c $< -o $@.o
	$(CXX) $@.o instrumentator.o -o $@.exe
instrumentator: instrumentator.cc
	$(CXX) -c $< -o $@.o 
