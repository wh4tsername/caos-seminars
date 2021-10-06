#include <stdio.h>
#include <stdint.h>

extern uint64_t R;

uint64_t A = 10;
uint64_t B = -3;

uint64_t C = 15;
uint64_t D = 3;

void calculate();

int main() {
  calculate();

  printf("%lu\n", R);

  return 0;
}

