#include <stdio.h>

int
fibo(int x) {
  if (x <= 1) {
    return 1;
  }
  return fibo(x-1) + fibo(x-2);
}


int
main(int argc, char** argv) {
  int result = fibo(10);
  printf("Fibonacci: %d\n", result);
  return 0;
}
