#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
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

    if (mkfifo(fifo_path, S_IRUSR | S_IWUSR) != 0) {
        printf("Error while creating fifo occurred.\n");
        exit(1);
    }

    fifo = open(fifo_path, O_RDONLY);
    if (fifo == -1) {
        printf("Error while opening fifo.\n");
        exit(1);
    }

    int line_size = 128;
    char *line = malloc(line_size * sizeof(char));
    time_t rawtime;
    char time_str[20];
    while (1) {
        if (readline(fifo, line, line_size, 1) > 0) {
            time(&rawtime);
            strftime(time_str, 20, "%F %T", localtime(&rawtime));
            printf("%s - %s\n", time_str, line);
        }
        else {
            sleep(0.5);
        }
    }
}

void sigint_handler(int signum) {
    if (fifo >= 0)
        close(fifo);
    unlink(fifo_path);
    exit(0);
}
