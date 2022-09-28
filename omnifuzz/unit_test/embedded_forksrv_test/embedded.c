#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int
main(int argc, char** argv) {
  char buf[21];
  int r = read(0, buf, 20);
  if (r == 0) 
    exit(-1);
  buf[r] = '\0';
  // FILE* fp = fopen("./tmp", "w+");

  // fprintf(fp, "[Program]: %s\n", buf);
  if (buf[0] == 'h')
    if (buf[1] == 'e')
      if (buf[2] == 'l')
        if (buf[3] == 'l')
          if (buf[4] == 'o')
            puts("hello world");
  return 0; 
}
