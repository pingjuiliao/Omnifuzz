#include <cstdlib>
#include <iostream>
#include "omnifuzz/preprocessor/instrumentator.h"
using namespace std;

int main(int argc, char** argv) {
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <source file>" << endl; 
    exit(-1);
  }
  
  omnifuzz::Instrumentator instr("/home/pingjui/Omnifuzz/build/");
  int r = instr.Instrument(argv[1]);
  if (r == 0) {
    cout << "Succeeded" << endl;
  }
  return 0;
}
