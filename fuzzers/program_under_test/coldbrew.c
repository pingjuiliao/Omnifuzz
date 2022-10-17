#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define STDIN 0
#define BUF_SIZE 20

int 
main(int argc, char** argv) {
  char buf[BUF_SIZE];
  int r = read(STDIN, buf, BUF_SIZE); 
  if (r == 0) {
    fprintf(stderr, "[Error] Program under test failed on read()");
    exit(-1);
  }
  
  if (buf[0] == 'c')
    if (buf[1] == 'o')
      if (buf[2] == 'l')
        if (buf[3] == 'd')
          if (buf[4] == 'b')
            if (buf[5] == 'r')
              if (buf[6] == 'e')
                if (buf[7] == 'w')
                  __builtin_trap();

  return 0;
}
