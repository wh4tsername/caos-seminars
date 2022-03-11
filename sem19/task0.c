#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  double* array;
  pthread_mutex_t* mutex;
  pthread_t thread;
  int N;
  int k;
  int index;
} context_t;

void* thread_func(void* arg) {
  context_t* ctx = arg;

  const int thread_position = ctx->index;
  const int left = thread_position == 0 ? ctx->k - 1 : thread_position - 1;
  const int right = thread_position == ctx->k - 1 ? 0 : thread_position + 1;
  const double left_add = 0.99;
  const double center_add = 1;
  const double right_add = 1.01;

  for (int i = 0; i < ctx->N; ++i) {
    pthread_mutex_lock(&ctx->mutex[left]);
    ctx->array[left] += left_add;
    pthread_mutex_unlock(&ctx->mutex[left]);

    pthread_mutex_lock(&ctx->mutex[thread_position]);
    ctx->array[thread_position] += center_add;
    pthread_mutex_unlock(&ctx->mutex[thread_position]);

    pthread_mutex_lock(&ctx->mutex[right]);
    ctx->array[right] += right_add;
    pthread_mutex_unlock(&ctx->mutex[right]);
  }

  return 0;
}

void init_threads(int k, int N, context_t* threads, double* array,
                  pthread_mutex_t* mutex) {
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);
  pthread_attr_setguardsize(&attr, 0);

  for (int i = 0; i < k; ++i) {
    threads[i].k = k;
    threads[i].N = N;
    threads[i].index = i;
    threads[i].mutex = mutex;
    threads[i].array = array;
    pthread_create(&threads[i].thread, &attr, thread_func, &threads[i]);
  }

  pthread_attr_destroy(&attr);
}

int main(int argc, char** argv) {
  int N = (int)strtol(argv[1], NULL, 10);
  int k = (int)strtol(argv[2], NULL, 10);

  context_t threads[k];
  double array[k];
  memset(array, 0, sizeof(array));

  pthread_mutex_t mutex[k];
  for (int i = 0; i < k; ++i) {
    pthread_mutex_init(&mutex[i], NULL);
  }

  init_threads(k, N, threads, array, mutex);

  for (int i = 0; i < k; ++i) {
    pthread_join(threads[i].thread, NULL);
  }
  for (int i = 0; i < k; ++i) {
    printf("%.10g\n", array[i]);
  }

  for (int i = 0; i < k; ++i) {
    pthread_mutex_destroy(&mutex[i]);
  }

  return 0;
}

