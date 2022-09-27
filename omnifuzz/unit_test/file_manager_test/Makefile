CXX=g++
CXXFLAGS=-I../../.. -std=c++17
all: mngr_build_tree	
mngr_build_tree: mngr_build_tree.cc TFM
	$(CXX) $(CXXFLAGS) -c $< -o $@.o
	$(CXX) $@.o TFM.o -o $@.exe
TFM: ../../testcase_file_manager.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@.o
clean:
	rm $(wildcard *.o) $(wildcard *.exe) 
	rm -rf $(wildcard ./omnifuzz_out)
