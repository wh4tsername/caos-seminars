#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define conditional_handle_error(stmt, msg) \
  if (stmt) {                               \
    perror(msg " (" #stmt ")");             \
    exit(EXIT_FAILURE);                     \
  }

#define MAX_QUERY_LEN 4096
#define MAX_CONTENT_LEN 60000

int send_query(char* host, char* path) {
  struct addrinfo hints = {.ai_family = AF_INET, .ai_socktype = SOCK_STREAM};
  struct addrinfo* addr = NULL;
  getaddrinfo(host, "http", &hints, &addr);

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  conditional_handle_error(sock == -1, "can't create socket");
  conditional_handle_error(connect(sock, addr->ai_addr, addr->ai_addrlen) != 0,
                           "can't connect");

  char query[MAX_QUERY_LEN];
  snprintf(query, sizeof(query),
           "GET %s HTTP/1.1\n"
           "Host: %s\n"
           "Connection: close\n"
           "\n",
           path, host);

  size_t query_size =
      strlen(query) < sizeof(query) ? strlen(query) : sizeof(query);

  conditional_handle_error(write(sock, query, query_size) == -1, "write error");

  return sock;
}

void read_and_print_content(int sock) {
  FILE* input = fdopen(sock, "r");
  conditional_handle_error(input == NULL, "enable to open socket as file");

  char buffer[MAX_CONTENT_LEN];
  bool has_met_blank_line = false;
  while (fgets(buffer, sizeof(buffer), input)) {
    if (strcmp(buffer, "\n") == 0 || strcmp(buffer, "\r\n") == 0) {
      has_met_blank_line = true;
    } else if (has_met_blank_line) {
      printf("%s", buffer);
      fflush(input);
    }
  }

  fclose(input);
}

int main(int argc, char** argv) {
  int sock = send_query(argv[1], argv[2]);

  read_and_print_content(sock);

  close(sock);

  return 0;
}

