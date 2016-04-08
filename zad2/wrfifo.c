#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include "main_lib.h"

void sigint_handler(int signum);

char *fifo_path;
int fifo;

int main(int argc, char *argv[]) {
    char * args_help = "Enter path to fifo file.\n";
    if (read_args(argc, argv, &fifo_path) != 0) {
        printf(args_help);
        return 1;
    }

    struct sigaction act;
    act.sa_handler = sigint_handler;
    sigaction(SIGINT, &act, NULL);

    fifo = open(fifo_path, O_WRONLY);
    if (fifo == -1) {
        printf("Error while opening fifo.\n");
        exit(1);
    }
    time_t rawtime;
    int line_size = 128;
    char *line = malloc(line_size * sizeof(char));
    char *line_ready = malloc(line_size * sizeof(char));
    char time_str[20];
    while (readline(STDIN_FILENO, line, line_size, 0)) {
        time(&rawtime);
        strftime(time_str, 20, "%F %T", localtime(&rawtime));
        sprintf(line_ready, "%d - %s - %s", getpid(), time_str, line);
        // if line was too long, then there is no \n char, so we have to add it
        if (line_ready[line_size-2] != '\0')
            line_ready[line_size-2] = '\n';

        if (write(fifo, line_ready, strlen(line_ready)) != strlen(line_ready)) {
            printf("Error while writing to fifo occurred.\n");
            close(fifo);
            exit(1);
        }
    }
    free(line);
    free(line_ready);
    close(fifo);

    return 0;
}

void sigint_handler(int signum) {
    if (fifo >= 0)
        close(fifo);
    exit(0);
}
