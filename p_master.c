#define _DEFAULT_SOURCE 

#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "include/slaveADT.h"

#define R 0
#define W 1
#define SHM_KEY 1234#define FILES_PER_SLAVE 2

int main(int argc, char const *argv[]) {
    
    struct shmseg *pShm;

    if (argc == 1) {
        fprintf(stderr, "No files inputted. \n");
        exit(1);
    }

//    int n = (argc + (argc % 2)) / FILES_PER_SLAVE, files = 0;
    int n = 1, m2s[2], s2m[2];
    slavesADT slaves = newSlaves();
    if(slaves == NULL)
        exit(1);
    for(int i = 0; i < n; i++){

        if(pipe(m2s) == -1 || pipe(s2m) == -1)
            exit(1);
        int pid = fork();
        if(pid < 0)
            exit(1);
        if(pid == 0){
            dup2(m2s[R], STDIN_FILENO);
            execlp("./p_slave", "p_slave", NULL);
        }
        close(m2s[R]);
        close(s2m[W]);
        addSlave(slaves, i, s2m[R], m2s[W]);
    }
    char newline = '\n', eof = EOF;
//    for(int i = 0; i < n; i++){
//        int fd = getWriteFD(slaves, i);
//        write(fd, argv[i + 1], PIPE_BUF);
//        write(fd, &newline, 1);
//    }
    write(getWriteFD(slaves, 0), argv[1], PIPE_BUF);
    write(getWriteFD(slaves, 0), &newline, 1);
    return 0;
}
