#include <stdio.h>
#include <stdint.h>
#include <limits.h>

typedef union {
  float real;
  uint32_t uint;
} real_or_uint;

int main() {
  float a = -1.5;

  printf("%f\n", a);

  real_or_uint u;
  u.real = a;

  uint32_t b = u.uint;

  int i = 0;
  while (i != 32) {
      if (i == 9 || i == 1) {
        printf(" ");
      }

      printf("%d", (b >> (31 - i)) % 2);

      ++i;
  }

  printf("\n");

  return 0;
}

