CXX=g++
CXXASAN=-fsanitize=address -static-libasan
CXXFLAGS=-I../../.. -std=c++17 $(CXXASAN)
all: a
b: b.cc timer 
	$(CXX) $(CXXFLAGS) -c $< -o $@.o
	$(CXX) $(CXXFLAGS) $@.o timer.o -o $@.exe
a: a.cc timer 
	$(CXX) $(CXXFLAGS) -c $< -o $@.o
	$(CXX) $(CXXFLAGS) $@.o timer.o -o $@.exe
timer: ../../timer.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@.o
clean:
	rm $(wildcard *.o) $(wildcard *.exe) 
