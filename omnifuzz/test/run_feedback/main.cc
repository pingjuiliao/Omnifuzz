#include <iostream>
#include "Omnifuzz/Fuzzers/afl_feedback.h"
using namespace std;

int main(int argc, char** argv) {
  omnifuzz::AFLFeedback fb;
  cout << "main program started\n";
  
  fb.operationOnBasicBlock(10);

  return 0;
}
