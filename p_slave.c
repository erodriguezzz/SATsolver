#define _DEFAULT_SOURCE 

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_LENGTH 1024
#define MAX_PATH_LENGTH 256
#define CLEAN_BUFF while(getchar() != '\n');

int main() {
    char buff[MAX_LENGTH], output[MAX_LENGTH], path[MAX_PATH_LENGTH];
    while(scanf("%s", path) != EOF) {
        CLEAN_BUFF
        sprintf(output, "%d\n", getpid());
        snprintf(buff, MAX_PATH_LENGTH,
                "minisat %s | grep -o -e \"Number of .*[0-9]\\+\" -e \"CPU time.*\" -e \".*SATISFIABLE\"", path);
        FILE *stream = popen(buff, "r");

        while (fgets(buff, MAX_LENGTH, stream) != NULL) {
            strcat(output, buff);
        }

        pclose(stream);
        fputs(output, stdout);
    }

    return 0;
}
