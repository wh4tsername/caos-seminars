#include <stdint.h>

uint64_t R;

extern uint64_t A;
extern uint64_t B;
extern uint64_t C;
extern uint64_t D;

int calculate() {
  asm("mov x1, %1\n\t"
      "mov x2, %2\n\t"
      "mov x3, %3\n\t"
      "mov x4, %4\n\t"

      "mov x0, #0\n\t"
      "mul x1, x1, x2\n\t"
      "mul x3, x3, x4\n\t"
      "add x0, x1, x3\n\t"
      "mov %0, x0\n\t"
      : "=r"(R)
      : "rm"(A), "rm"(B), "rm"(C), "rm"(D)
      : "memory");

  return 0;
}

