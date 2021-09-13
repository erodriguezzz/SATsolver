#define _DEFAULT_SOURCE 

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#define MAX_LENGTH 1024
#define MAX_PATH_LENGTH 256
#define CLEAN_BUFF while(getchar() != '\n');

int main() {
    char buff[MAX_LENGTH], output[PIPE_BUF], path[MAX_PATH_LENGTH];
    while(scanf("%s", path) != EOF) {
        sprintf(output, "%d\tFILENAME: ", getpid());
        CLEAN_BUFF
        strcat(output, path);
        strcat(output, "\t");
        snprintf(buff, MAX_LENGTH,
                "minisat %s | grep -o -e \"Number of .*[0-9]\\+\" -e \"CPU time.*\" -e \".*SATISFIABLE\" | tr '\\n' '\\t'", path);
        FILE *stream = popen(buff, "r");
        while (fgets(buff, MAX_LENGTH, stream) != NULL) {
            strcat(output, buff);
        }
        pclose(stream);
        printf("%s\n", output);
        fflush(NULL);
    }

    return 0;
}
