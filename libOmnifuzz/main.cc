#include <stdio.h>
#include "libOmnifuzz.h"

int main(int argc, char** argv) {
  _omnifuzz_basicblock(0);
  _omnifuzz_basicblock(1);
  _omnifuzz_basicblock(2);
  _omnifuzz_basicblock(3);

  return 0;
}
