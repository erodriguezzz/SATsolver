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

int main(int argc, char const *argv[]){
    if(argc == 1){
        fprintf(stderr, "No files inputted. \n");
        exit(1);
    }

    char shm_name[MAX_NAME_LENGTH] = SHM_NAME; char sem_name[MAX_NAME_LENGTH] = SEM_NAME;

    shmADT shm = newShm(shm_name, sem_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shm == NULL) {
        return -1; // TODO: Err exit.
    }

    puts(shm_name);
    puts(sem_name);
    fflush(stdout);
    sleep(SLEEP_TIME);

    char n_files[FILE_BUF_LEN] = {0};
    int read = sprintf(n_files, "%d", argc - 1);
    if (read < 0) {
        exit(1); // TODO: Err exit.
    }
    if (writeShm(shm, n_files, read+1) == -1) {
        closeShm(shm, true);
        return 1; // TODO: Err exit.
    }

    int n = (argc + FILES_PER_SLAVE - 2) /  FILES_PER_SLAVE, sent = 0, received = 0;
    slavesADT slaves = newSlaves();
    if(slaves == NULL) {
        exit(1);
    }
    int m2s[2], s2m[2]; // Master to slave, slave to master.
    if(pipe(s2m) < 0) {
        exit(1); // TODO: Err exit.
    }
    int i;
    for (i = 0; i < n; i++) {
        if(pipe(m2s) < 0)
            exit(1);
        int pid = fork();
        if(pid < 0)
            exit(1);
        if(pid == 0){
            closeAllFD(slaves);
            close(m2s[P_W]); // TODO: Err exit.
            close(s2m[P_R]);
            dup2(m2s[P_R], STDIN_FILENO);// TODO: Err exit.
            dup2(s2m[P_W], STDOUT_FILENO);
            close(m2s[P_R]);
            close(s2m[P_W]);
            execl(PATH_TO_SLAVE, SLAVE, (char *) NULL);
            return 1;
        }
        close(m2s[P_R]);
        if(addSlave(slaves, pid, m2s[P_W]) == 0) {
            exit(1); // TODO: Err exit.
        }
        dprintf(m2s[P_W], "%s\n", argv[i + 1]); // TODO: Err exit.
        sent++;
    }
    close(s2m[P_W]);

    FILE * s2m_ptr = fdopen(s2m[P_R], "r");
    FILE * result = fopen("result.txt", "w+");
    if (result == NULL) {
        return -1; // TODO: Err exit.
    }
    char *buf = NULL;
    size_t len;

    while (received < argc - 1) {
        getline(&buf, &len, s2m_ptr); // TODO: Err exit.
        writeShm(shm, buf, len + 1);
        fprintf(result, "%s", buf);
        received++;
        int id = atoi(buf);
        int fd = getWriteFD(slaves, id);
        if (sent < argc - 1) {
            dprintf(fd, "%s\n", argv[sent + 1]); // TODO: Err exit.
            sent++;
        } else {
            rmSlave(slaves, id); // TODO: Err exit.
            close(fd);
        }
    }

    free(buf);
    fclose(s2m_ptr);
    fclose(result);
    freeSlaves(slaves);
    closeShm(shm, true);
    return 0;
}