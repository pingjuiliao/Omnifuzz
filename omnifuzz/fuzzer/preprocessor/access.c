#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char** argv) {
  if (access("./a.c", F_OK)) 
  {
    puts("a.c exists");
  }
  return 0;

}
