#include <stdio.h>
#include <stdint.h>
#include <limits.h>

int main() {
  float a = -1.5;

  printf("%f\n", a);

  float* a_ptr_as_float = &a;

  void* a_ptr_as_void = a_ptr_as_void;

  uint32_t* a_ptr_as_uint = a_ptr_as_void;

  uint32_t b = *a_ptr_as_uint;

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

