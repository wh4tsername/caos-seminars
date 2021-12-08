#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
    const size_t MAX_INPUT_SIZE = 10000;
    char temp[MAX_INPUT_SIZE];
    char program[MAX_INPUT_SIZE];

    fgets(temp, sizeof(temp), stdin);

    char* delimiter = strchr(temp, '\n');
    if (delimiter != NULL) {
        *delimiter = '\0';
    }

    if (strnlen(temp, sizeof(temp)) == 0) {
        return 0;
    }

    char* format_string = "print(%s)";
    snprintf(program, sizeof(program), format_string, temp);
    execlp("python3", "python3", "-c", program, NULL);
    perror("failed exec");
    exit(1);
}
