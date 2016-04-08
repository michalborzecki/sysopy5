#include <stdio.h>
#include <unistd.h>
#include "main_lib.h"

size_t readline(int fd, char * buffer, int size, int cut_new_line) {
    char c;
    int counter = 0;
    if (!cut_new_line)
        size--;
    while (read(fd, &c, 1) != 0 && counter < size - 1) {
        if (c == '\n') {
            break;
        }
        buffer[counter++] = c;
    }
    if (!cut_new_line) {
        buffer[counter++] = '\n';
    }
    buffer[counter] = '\0';
    return counter;
}

int read_args(int argc, char *argv[], char **fifo_path) {
    if (argc != 2) {
        printf("Incorrect number of arguments.\n");
        return 1;
    }
    *fifo_path = argv[1];
    return 0;
}
