#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <unistd.h>

#define conditional_handle_error(stmt, msg)                                    \
    do {                                                                       \
        if (stmt) {                                                            \
            perror(msg " (" #stmt ")");                                        \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
    } while (0);

extern size_t read_data_and_count(size_t N, int in[N])
{
    for (int i = 0; i < N; ++i) {
        fcntl(in[i], F_SETFL, fcntl(in[i], F_GETFL) | O_NONBLOCK);
    }

    int epoll_fd = epoll_create1(0);
    conditional_handle_error(epoll_fd == -1, "epoll create error");

    for (int i = 0; i < N; ++i) {
        struct epoll_event event = {.events = EPOLLIN, .data.fd = in[i]};
        conditional_handle_error(
            epoll_ctl(epoll_fd, EPOLL_CTL_ADD, in[i], &event) == -1,
            "epoll ctl error");
    }

    int total_bytes = 0;
    int number_of_opened = N;
    const size_t MAX_SIZE = 4096;
    char buffer[MAX_SIZE];
    struct epoll_event events[N];
    while (number_of_opened != 0) {
        int current_count = epoll_wait(epoll_fd, events, N, -1);

        for (int i = 0; i < current_count; ++i) {
            int read_bytes = read(events[i].data.fd, buffer, MAX_SIZE);

            if (read_bytes > 0) {
                total_bytes += read_bytes;
            } else {
                close(events[i].data.fd);
                --number_of_opened;
            }
        }
    }

    close(epoll_fd);

    return total_bytes;
}
