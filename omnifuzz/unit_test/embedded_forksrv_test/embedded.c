#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int
main(int argc, char** argv) {
  pid_t pid = getpid();
  if (pid % 2 == 0)
    if (pid % 3 == 0)
      if (pid % 5 == 0)
        if (pid % 7 == 0)
          puts("hello world");
  return 0; 
}
