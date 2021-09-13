#define _DEFAULT_SOURCE

#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "include/slaveADT.h"

#define R 0
#define W 1
#define SHM_KEY 1234
#define FILES_PER_SLAVE 2

// TODO: add more meaningful error handling.
int main(int argc, char const *argv[]){
    if(argc == 1){
        fprintf(stderr, "No files inputted. \n");
        exit(1);
    }
    int n = (argc + FILES_PER_SLAVE - 2) /  FILES_PER_SLAVE, sent = 0, received = 0;
    printf("Number of slaves: %d\n", n);
    slavesADT slaves = newSlaves();
    if(slaves == NULL) {
        exit(1);
    }
    int m2s[2], s2m[2]; // Master to slave, slave to master.
    if(pipe(s2m) < 0)
        exit(1);
    for(int i = 0; i < n; i++){
        if(pipe(m2s) < 0)
            exit(1);
        int pid = fork();
        if(pid < 0)
            exit(1);
        if(pid == 0){
            close(m2s[W]);
            close(s2m[R]);
            dup2(m2s[R], STDIN_FILENO);
            dup2(s2m[W], STDOUT_FILENO);
            execl("./p_slave", "p_slave", (char *) NULL);
            return 1;
        }
        close(m2s[R]);
        if(addSlave(slaves, pid, m2s[W]) == 0) {
            exit(1);
        }
        dprintf(m2s[W], "%s\n", argv[i + 1]);
        sent++;
    }
    close(s2m[W]);

    FILE * s2m_ptr = fdopen(s2m[R], "r");
    FILE * result = fopen("result.txt", "w+");
    char *buf = NULL;
    size_t len;

    while(received < argc - 1) {
        getline(&buf, &len, s2m_ptr);
        fprintf(result, "%s", buf);
        received++;
        int id = atoi(buf);
        int fd = getWriteFD(slaves, id);
        if (sent < argc - 1) {
            dprintf(fd, "%s\n", argv[sent + 1]);
            sent++;
        } else {
            close(fd);
        }
    }
    free(buf);
    fclose(s2m_ptr);
    fclose(result);
    freeSlaves(slaves);
    return 0;
}