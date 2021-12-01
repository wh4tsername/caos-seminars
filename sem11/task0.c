#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    char* file_name = argv[1];
    char* pattern = argv[2];
    int fd = open(file_name, O_RDONLY);
    struct stat st;
    fstat(fd, &st);
    size_t file_size = st.st_size;
    if (0 == file_size) {
        close(fd);
        exit(0);
    }
    void* file_begin = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    void* cur_pos = file_begin;
    size_t pattern_len = strlen(pattern);
    size_t diff = 0;
    while (NULL != (cur_pos = memmem(
                        cur_pos, file_size - diff, pattern, pattern_len))) {
        diff = cur_pos - file_begin;
        printf("%lu ", diff);
        cur_pos++;
    }
    printf("\n");
    munmap(file_begin, file_size);
    close(fd);
}

