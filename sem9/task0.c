#include <fcntl.h>
#include <unistd.h>

int FilterFile(int input_file, int digits_file, int other_symbols_file)
{
    if (input_file == -1) {
        return 1;
    }

    if (digits_file == -1 || other_symbols_file == -1) {
        return 2;
    }

    ssize_t scanned = 0;
    ssize_t written = 0;
    int file_to_write = 0;
    char read_byte;

    scanned = read(input_file, &read_byte, sizeof(read_byte));

    while (scanned > 0) {
        file_to_write = read_byte >= '0' && read_byte <= '9'
                            ? digits_file
                            : other_symbols_file;

        written = write(file_to_write, &read_byte, sizeof(read_byte));

        if (written == -1) {
            return 3;
        }

        scanned = read(input_file, &read_byte, sizeof(read_byte));
    }

    return scanned == -1 ? 3 : 0;
}

int main(int argc, char* argv[])
{
    int input_file = open(argv[1], O_RDONLY);
    int digits_file = open(argv[2], O_WRONLY | O_CREAT, 0640);
    int other_symbols_file = open(argv[3], O_WRONLY | O_CREAT, 0640);

    int exit_code = FilterFile(input_file, digits_file, other_symbols_file);

    close(input_file);
    close(digits_file);
    close(other_symbols_file);

    return exit_code;
}
