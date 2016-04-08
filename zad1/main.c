#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

int read_args(int argc, char *argv[], int *n);

int main(int argc, char *argv[]) {
    int n;
    char * args_help = "Enter n: cols number.\n";
    // Only have to check, if arg n is a number. Variable n is not used later.
    if (read_args(argc, argv, &n) != 0) {
        printf(args_help);
        return 1;
    }

    int pipefd[2];
    if (pipe(pipefd) != 0) {
        printf("Error while creating pipe occurred.\n");
        exit(1);
    }

    // creating process for tr
    pid_t tr_pid = fork();
    if (tr_pid < 0) {
        printf("Error while forking process occurred.\n");
        exit(1);
    }
    else if (tr_pid > 0) {
        // creating process for fold
        pid_t fold_pid = fork();
        if (fold_pid < 0) {
            kill(tr_pid, SIGINT); // close tr process
            printf("Error while forking process occurred.\n");
            exit(1);
        }
        else if (fold_pid > 0) { // parent process
            close(pipefd[0]);
            close(pipefd[1]);
            wait(NULL);
            wait(NULL);
        }
        else { // fold process
            dup2(pipefd[0], 0);
            close(pipefd[0]);
            close(pipefd[1]);
            execlp("fold", "fold", "-w", argv[1], NULL);
        }
    }
    else { // tr process
        dup2(pipefd[1], 1);
        close(pipefd[0]);
        close(pipefd[1]);
        execlp("tr", "tr", "'[:lower:]'", "'[:upper:]'", NULL);
    }

    return 0;
}

int read_args(int argc, char *argv[], int *n) {
    if (argc != 2) {
        printf("Incorrect number of arguments.\n");
        return 1;
    }
    *n = atoi(argv[1]);
    if (*n <= 0) {
        printf("Incorrect number for fold. It should be > 0.\n");
        return 1;
    }
    return 0;
}
