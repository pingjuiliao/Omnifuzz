CXX=g++

all: main 
a: 
	echo "'a' file should be the input of the main.exe"
main: main.cc instrumentator
	$(CXX) -c $< -o $@.o
	$(CXX) $@.o instrumentator.o -o $@.exe
instrumentator: instrumentator.cc
	$(CXX) -c $< -o $@.o 
