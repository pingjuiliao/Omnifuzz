CXX=g++
CXXFLAGS=-I../../..
all: main 
a: 
	echo "'a' file should be the input of the main.exe"
main: main.cc instrumentator
	$(CXX) $(CXXFLAGS) -c $< -o $@.o
	$(CXX) $@.o instrumentator.o -o $@.exe
instrumentator: ../instrumentator.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@.o 
clean:
	rm $(wildcard *.o) $(wildcard *.exe)

