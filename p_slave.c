#define _DEFAULT_SOURCE 

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_LENGTH 1024
#define READ 0
#define WRITE 1


int main(int argc, const char *argv[]) {
    if (argc == 1) {
        return -1;
    }
    char cmd[MAX_LENGTH];

    while(--argc) {
        sprintf(cmd, "minisat %s |  grep -o -e \"Number of .*[0-9]\\+\" -e \"CPU time.*\" -e \".*SATISFIABLE\"", argv[argc]);
        FILE *stream = popen(cmd, "r");
        while (fgets(cmd, MAX_LENGTH, stream) != NULL) {
            fputs(cmd, stdout);
        }
        pclose(stream);
    }

    return 0;
}
