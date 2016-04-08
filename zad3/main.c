#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int read_args(int argc, char *argv[], char **output);

int main(int argc, char *argv[]) {
    char *output;
    char *args_help = "Enter path to output file.\n";
    if (read_args(argc, argv, &output) != 0) {
        printf(args_help);
        return 1;
    }
    int output_fd = open(output, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    if (output_fd < 0) {
        printf("Error while opening output occurred.");
        exit(1);
    }
    dup2(output_fd, 1);
    close(output_fd);
    FILE *command_pipe = popen("ls -l | grep ^d", "w");
    if (command_pipe == NULL) {
        printf("Error while running command occurred.\n");
        exit(1);
    }
    pclose(command_pipe);
    close(1);

    return 0;
}

int read_args(int argc, char *argv[], char **output) {
    if (argc != 2) {
        printf("Incorrect number of arguments.\n");
        return 1;
    }
    *output = argv[1];
    return 0;
}
