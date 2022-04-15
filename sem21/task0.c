#include <dlfcn.h>
#include <stdio.h>

typedef double (*function_t)(double);

int main (int argc, char** argv) {
  void* lib_handle = dlopen(argv[1], RTLD_NOW);
  function_t function = dlsym(lib_handle, argv[2]);

  double number;
  while (scanf("%lf", &number) != EOF) {
    printf("%lf ", function(number));
  }

  dlclose(lib_handle);

  return 0;
}

