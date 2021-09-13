// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#define _DEFAULT_SOURCE 

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>

#define MAX_PATH_LENGTH 256
#define CLEAN_BUFF while(getchar() != '\n');

int isValidPath(const char * path);

int main() {
    char buff[PIPE_BUF] = {0}, output[PIPE_BUF], path[MAX_PATH_LENGTH];
    char fmt[MAX_PATH_LENGTH];
    sprintf(fmt, "%%%ds", MAX_PATH_LENGTH);
    while(scanf(fmt, path) != EOF) {
        sprintf(output, "%d\tFILENAME: ", getpid());
        CLEAN_BUFF
        strcat(output, path);
        strcat(output, "\t");
        FILE * stream = NULL;
        if (isValidPath(path)) {
            if (snprintf(buff, PIPE_BUF,
                         "minisat %s | grep -o -e \"Number of .*[0-9]\\+\" -e \"CPU time.*\" -e \".*SATISFIABLE\" | tr '\\n' '\\t'", path) > 0) {
                stream = popen(buff, "r");
            } else {
                return -1;
            }
        } else {
            return -1;
        }
        while (fgets(buff, PIPE_BUF, stream) != NULL) {
            strcat(output, buff);
        }
        pclose(stream);
        printf("%s\n", output);
        fflush(stdout);
    }

    return 0;
}


int isValidPath(const char * path) {
    int fd = 0;
    if ((fd = open(path, O_RDONLY, S_IRUSR)) == -1) {
        return 0;
    }
    close(fd);
    return 1;
}
