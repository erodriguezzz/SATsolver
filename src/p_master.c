// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/**
 * @file: p_master.c
 * @authors: Ezequiel Rodriguez, Juan I. Garcia M. & Jerónimo Brave.
 * */

/* Local includes */
#include "include/slaveADT.h"
#include "include/shmADT.h"

/* Standard & Sys lib's */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* Constants */
#define P_R 0
#define P_W 1
#define FILES_PER_SLAVE 2
#define FILE_BUF_LEN 10

#define SHM_NAME "0x12345678"
#define SEM_NAME "0x54321012"

#define PATH_TO_SLAVE "./p_slave"
#define SLAVE "p_slave"

#define SLEEP_TIME 3

#define FAILURE 1
#define SUCCESS 0

void errorMessage(int exit_status, char *msg);

int main(int argc, char const *argv[]){
    if(argc == 1){
        fprintf(stderr, "No files inputted.\n");
        exit(FAILURE);
    }

    char shm_name[MAX_NAME_LENGTH] = SHM_NAME; char sem_name[MAX_NAME_LENGTH] = SEM_NAME;

    shmADT shm = newShm(shm_name, sem_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shm == NULL)
        errorMessage(FAILURE, "Error in function newShm()");

    puts(shm_name);
    puts(sem_name);
    fflush(stdout);
    sleep(SLEEP_TIME);

    char n_files[FILE_BUF_LEN] = {0};
    int read = sprintf(n_files, "%d", argc - 1);
    if (read < 0)
        errorMessage(FAILURE, "Error in function sprintf()");

    if (writeShm(shm, n_files, read+1) == -1)
        errorMessage(FAILURE, "Error in function write()");

    int n = (argc + FILES_PER_SLAVE - 2) /  FILES_PER_SLAVE, sent = 0, received = 0;
    slavesADT slaves = newSlaves();
    if(slaves == NULL)
        errorMessage(FAILURE, "Error in function newSlaves()");
    int m2s[2], s2m[2]; // Master to slave, slave to master.
    if(pipe(s2m) < 0)
        errorMessage(FAILURE, "Error in function pipe()");
    int i;
    for (i = 0; i < n; i++) {
        if(pipe(m2s) < 0)
            errorMessage(FAILURE, "Error in function pipe()");
        int pid = fork();
        if(pid < 0)
            errorMessage(FAILURE, "Error in function fork()");
        if(pid == 0){
            closeAllFD(slaves);
            close(m2s[P_W]);
            close(s2m[P_R]);
            dup2(m2s[P_R], STDIN_FILENO);
            dup2(s2m[P_W], STDOUT_FILENO);
            close(m2s[P_R]);
            close(s2m[P_W]);
            execl(PATH_TO_SLAVE, SLAVE, (char *) NULL);
            errorMessage(FAILURE, "Error in function execl()");
        }
        close(m2s[P_R]);
        if(addSlave(slaves, pid, m2s[P_W]) == 0) {
            errorMessage(FAILURE, "Error in function addSlave()");
        }
        if(dprintf(m2s[P_W], "%s\n", argv[i + 1]) < 0)
            errorMessage(FAILURE, "Error in function dprintf()");
        sent++;
    }
    close(s2m[P_W]);

    FILE * s2m_ptr = fdopen(s2m[P_R], "r");
    if(s2m_ptr == NULL)
        errorMessage(FAILURE, "Error in function fdopen()");
    FILE * result = fopen("result.txt", "w+");
    if (result == NULL)
        errorMessage(FAILURE, "Error in function fopen()");
    char *buf = NULL;
    size_t len;

    while (received < argc - 1) {
        if(getline(&buf, &len, s2m_ptr) < 0)
            errorMessage(FAILURE, "Error in function getline()");
        if(writeShm(shm, buf, len + 1) < 0)
            errorMessage(FAILURE, "Error in function writeShm()");
        fprintf(result, "%s", buf);
        received++;
        int id = atoi(buf);
        int fd = getWriteFD(slaves, id);
        if(fd == -1){
            fprintf(stderr, "Slave pid %d is not stored.\n", id);
            exit(FAILURE);
        }
        if (sent < argc - 1) {
            if(dprintf(fd, "%s\n", argv[sent + 1]) < 0)
                errorMessage(FAILURE, "Error in function dprintf()");
            sent++;
        } else {
            rmSlave(slaves, id);
        }
    }

    free(buf);
    fclose(s2m_ptr);
    fclose(result);
    freeSlaves(slaves);
    closeShm(shm, true);
    return SUCCESS;
}

void errorMessage(int exit_status, char *msg){
    perror(msg);
    exit(exit_status);
}
