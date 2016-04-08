#ifndef ZAD1_MAIN_LIB_H
#define ZAD1_MAIN_LIB_H

size_t readline(int fd, char * buffer, int size, int cut_new_line);
int read_args(int argc, char *argv[], char **fifo_path);

#endif //ZAD1_MAIN_LIB_H
