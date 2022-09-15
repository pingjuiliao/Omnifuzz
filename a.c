#include <stdio.h>

void foo(void) {
  puts("hello world");
}

int
main(int argc, char** argv) {
  foo(); 
  puts("about to end");
  return 0;
}
