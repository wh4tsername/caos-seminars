#include <openssl/conf.h>
#include <openssl/sha.h>
#include <string.h>
#include <zconf.h>

#define BUFFER_SIZE 2000

typedef unsigned char* data_t;

typedef struct {
  data_t data;
  size_t size;
} buffer_t;

int main() {
  buffer_t hash = {.data = malloc(SHA512_DIGEST_LENGTH),
                   .size = SHA512_DIGEST_LENGTH};
  buffer_t plain = {.data = malloc(BUFFER_SIZE), .size = BUFFER_SIZE};

  SHA512_CTX* ctx = malloc(sizeof(SHA512_CTX));
  SHA512_Init(ctx);

  int bytes_read = -1;
  while (bytes_read != 0) {
    bytes_read = read(0, (char*)plain.data, plain.size);
    SHA512_Update(ctx, plain.data, bytes_read);
  }
  SHA512_Final(hash.data, ctx);

  printf("0x");
  for (size_t i = 0; i < hash.size; ++i) {
    printf("%02x", hash.data[i]);
  }
  printf("\n");

  free(hash.data);
  free(plain.data);
  free(ctx);

  return 0;
}

