#define _DEFAULT_SOURCE 

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define MAX_LENGTH 1024
#define MAX_PATH_LENGTH 256
#define READ 0
#define WRITE 1

int main(int argc, const char *argv[]) {
    char buff[MAX_LENGTH], output[MAX_LENGTH], path[MAX_PATH_LENGTH];
    sprintf(output, "%d\n", getpid());

    while(fgets(path, MAX_LENGTH, stdin) != NULL) {
        sprintf(buff, "minisat %s |  grep -o -e \"Number of .*[0-9]\\+\" -e \"CPU time.*\" -e \".*SATISFIABLE\"", path);
        FILE *stream = popen(buff, "r");
        
        while (fgets(buff, MAX_LENGTH, stream) != NULL) {
            strcat(output, buff);
        }
        
        pclose(stream);
        fputs(output, stdout);
    }

    return 0;
}
