#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

extern double very_important_function(size_t N, const float *A, const float *B, float *R);

int main() {
  size_t N = 8;
  float* A = aligned_alloc(32, N * (sizeof(*A)));
  float* B = aligned_alloc(32, N * (sizeof(*B)));

  for (size_t i = 0; i < N; ++i) {
      float fi = (float)(i+1);    
  
      A[i] = 1 / fi;
      B[i] = -1 / fi;
  }

  float* R = aligned_alloc(32, N * (sizeof(*R)));

  very_important_function(N, A, B, R);

  for (size_t i = 0; i < N; ++i) {
    printf("%f ", R[i]);
  }
  printf("\n");

  return 0;
}

