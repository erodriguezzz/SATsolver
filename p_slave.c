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
    //int count = 0;
    char cmd[MAX_LENGTH], c;
    while(--argc) {
        sprintf(cmd, "minisat %s |  grep -o -e \"Number of .*[0-9]\\+\" -e \"CPU time.*\" -e \".*SATISFIABLE\"", argv[argc]);
        FILE *stream = popen(cmd, "w");
        while((c = fgetc(stream)) != EOF){
            putchar(c);
        }
        pclose(stream);
    }
    return 0;
}