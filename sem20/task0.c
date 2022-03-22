#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define conditional_handle_error(stmt, msg)                                    \
    do {                                                                       \
        if (stmt) {                                                            \
            perror(msg " (" #stmt ")");                                        \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
    } while (0)

int main(int argc, char* argv[])
{
    short port_num = strtol(argv[1], NULL, 10);
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    conditional_handle_error(-1 == sock, "socket error");

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = inet_addr("127.0.0.1"),
        .sin_port = htons(port_num)};

    int send_num = 0;
    int recv_num = 0;
    while (scanf("%d", &send_num) > 0) {
        sendto(
            sock,
            &send_num,
            sizeof(send_num),
            0,
            (const struct sockaddr*)&addr,
            sizeof(addr));

        recvfrom(sock, &recv_num, sizeof(recv_num), 0, NULL, NULL);

        printf("%d\n", recv_num);
    }

    close(sock);
}
