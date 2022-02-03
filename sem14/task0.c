#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char** argv)
{
    char* cmd = argv[1];
    char* input_file = argv[2];

    int input_fd = open(input_file, O_RDONLY);
    dup2(input_fd, 0);
    close(input_fd);

    int fds[2];
    pipe(fds);

    pid_t pid = fork();
    if (pid == 0) {
        dup2(fds[1], 1);
        close(fds[1]);
        close(fds[0]);
        execlp(cmd, cmd, NULL);
    } else {
        close(fds[1]);
        const size_t BUFFER_SIZE = 4096;
        char buffer[BUFFER_SIZE];
        uint64_t total = 0;
        int size = 0;
        while ((size = read(fds[0], buffer, sizeof(buffer))) > 0) {
            total += size;
        }
        close(fds[0]);
        waitpid(pid, 0, 0);

        printf("%lu\n", total);
    }

    return 0;
}
