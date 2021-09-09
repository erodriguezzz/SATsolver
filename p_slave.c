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
    char buff[MAX_LENGTH], output[MAX_LENGTH];
    pid_t pid = getpid();

    while(fgets(buff, MAX_LENGTH, stdin) != NULL) {
        sprintf(output, "minisat %s | grep -o -e \"Number of .*[0-9]\\+\" -e \"CPU time.*\" -e \".*SATISFIABLE\"", buff);
        FILE *stream = popen(output, "r");
        puts("Hola");
        sprintf(output, "%d\n", pid);
        while (fgets(buff, MAX_LENGTH, stream) != NULL) {
            strcat(output, buff);
        }
        
        pclose(stream);
        printf(output);
    }

    return 0;
}
