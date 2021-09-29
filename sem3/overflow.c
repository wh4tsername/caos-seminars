#include <stdint.h>
#include <stdio.h>
#include <limits.h>

int check_inc(int32_t x) {
  return x + 1 > x;
}

int unsigned_check_inc(uint32_t x) {
  return x + 1 > x;
} 

int main() {
  int32_t max1 = INT_MAX;
  uint32_t max2 = UINT_MAX;

  printf("%d\n", check_inc(1));
  printf("%d\n", check_inc(max1));
  printf("%d\n", unsigned_check_inc(1));
  printf("%d\n", unsigned_check_inc(max2));

  return 0;
}

