#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    int32_t count = 1;
    while (true) {
        int pid = fork();

        if (pid < 0) {
            printf("%d\n", count);
            return 0;
        }

        if (pid != 0) {
            int status;
            waitpid(pid, &status, 0);
            break;
        }
        ++count;
    }

    return 0;
}
