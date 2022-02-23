#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define conditional_handle_error(stmt, msg)                                    \
    do {                                                                       \
        if (stmt) {                                                            \
            perror(msg " (" #stmt ")");                                        \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
    } while (0)

int get_port(char* port_str)
{
    return (int)strtol(port_str, NULL, 10);
}

struct in_addr get_addr(char* addr_str)
{
    struct in_addr addr_struct;
    int err_code = inet_aton(addr_str, &addr_struct);
    conditional_handle_error(err_code == 0, "incorrect address");

    return addr_struct;
}

int send_retry(
    const int connection_fd,
    const char* message,
    const int message_size)
{
    int total_bytes = 0;
    int bytes_sent = 0;

    while (total_bytes != message_size) {
        bytes_sent = send(
            connection_fd,
            &message[total_bytes],
            message_size - total_bytes,
            MSG_NOSIGNAL);

        if (bytes_sent == -1) {
            return bytes_sent;
        }

        total_bytes += bytes_sent;
    }

    return total_bytes;
}

int recv_retry(const int connection_fd, char* message, const int message_size)
{
    int total_bytes = 0;
    int bytes_received = 0;

    while (total_bytes != message_size) {
        bytes_received = recv(
            connection_fd,
            &message[total_bytes],
            message_size - total_bytes,
            0);

        if (bytes_received <= 0) {
            return bytes_received;
        }

        total_bytes += bytes_received;
    }

    return total_bytes;
}

void echo(int fd)
{
    int number = 0;
    while (true) {
        if (scanf("%d", &number) == EOF) {
            break;
        }

        int send_err_code =
            send_retry(fd, (const char*)&number, sizeof(number));
        if (send_err_code == -1) {
            break;
        }

        int recv_err_code = recv_retry(fd, (char*)&number, sizeof(number));
        if (recv_err_code == 0) {
            break;
        }
        printf("%d ", number);
    }
}

int main(int argc, char** argv)
{
    int connection_fd = socket(AF_INET, SOCK_STREAM, 0);
    conditional_handle_error(connection_fd == -1, "can't initialize socket");

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(get_port(argv[2]));
    address.sin_addr = get_addr(argv[1]);

    int connect_ret =
        connect(connection_fd, (struct sockaddr*)&address, sizeof(address));
    conditional_handle_error(connect_ret == -1, "can't connect to socket");

    echo(connection_fd);

    shutdown(connection_fd, SHUT_RDWR);
    close(connection_fd);

    return 0;
}
