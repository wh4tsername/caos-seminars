#include "stddef.h"

int *A;

void very_important_function(size_t N);

int main() {
  size_t N = 2;
  int a[N];
  
  a[0] = 1;
  a[1] = 2;

  A = a;
  very_important_function(N);

  return 0;
}

