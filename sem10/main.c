#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  char path[PATH_MAX + 1];
  uint64_t result = 0;
  while (fgets(path, sizeof(path), stdin)) {
    for (int i = 0; i < PATH_MAX + 1; ++i) {
      if (path[i] == '\n') {
        path[i] = '\0';
        break;
      }
    }

    struct stat s;
    memset(&s, 0, sizeof(s));

    if (lstat(path, &s) == 0) {
      if (S_ISREG(s.st_mode)) {
        result += s.st_size;
      }
    }
  }

  printf("%" PRIu64 "\n", result);

  return 0;
}

