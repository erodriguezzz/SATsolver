#define _DEFAULT_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "include/slaveADT.h"
#include "include/shmADT.h"

#define P_R 0 // TODO: Change this to P_R & P_W to avoid confusion with the shm perms & mode.
#define P_W 1
#define FILES_PER_SLAVE 2
#define FILE_BUF_LEN 10

#define SHM_NAME "0x12345678"
#define SEM_NAME "0x54321012"

// TODO: add more meaningful error handling.
int main(int argc, char const *argv[]){
    if(argc == 1){
        fprintf(stderr, "No files inputted. \n");
        exit(1);
    }

    char shm_name[MAX_NAME_LENGTH] = SHM_NAME;
    char sem_name[MAX_NAME_LENGTH] = SEM_NAME;

    shmADT shm = newShm(shm_name, sem_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    if (shm == NULL) {
        return -1;
    }

    // Flushear salida o pasar datos de shmem a la vista.
    puts(shm_name);
    puts(sem_name);

    fflush(stdout);

    char n_files[FILE_BUF_LEN] = {0};
    int read = sprintf(n_files, "%d", argc - 1);
    if (read < 0) {
        exit(1);
    }
    puts(n_files);
    if (writeShm(shm, n_files, read+1) == -1) {
        closeShm(shm, true);
        puts("Cagaste pa");
        return 1;
    }

    int n = (argc + FILES_PER_SLAVE - 2) /  FILES_PER_SLAVE, sent = 0, received = 0;
    slavesADT slaves = newSlaves();
    if(slaves == NULL) {
        exit(1);
    }
    int m2s[2], s2m[2]; // Master to slave, slave to master.
    if(pipe(s2m) < 0)
        exit(1);
    int i;
    for(i = 0; i < n; i++){
        if(pipe(m2s) < 0)
            exit(1);
        int pid = fork();
        if(pid < 0)
            exit(1);
        if(pid == 0){
            close(m2s[P_W]); 
            close(s2m[P_R]);
            dup2(m2s[P_R], STDIN_FILENO);
            dup2(s2m[P_W], STDOUT_FILENO);
            execl("./p_slave", "p_slave", (char *) NULL);
            return 1;
        }
        close(m2s[P_R]);
        if(addSlave(slaves, pid, m2s[P_W]) == 0) {
            exit(1);
        }
        dprintf(m2s[P_W], "%s\n", argv[i + 1]);
        sent++;
    }
    close(s2m[P_W]);

    FILE * s2m_ptr = fdopen(s2m[P_R], "r");
    FILE * result = fopen("result.txt", "w+");
    char *buf = NULL;
    size_t len;

    while(received < argc - 1) {
        getline(&buf, &len, s2m_ptr);
        writeShm(shm, buf, len + 1);
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

    getc(stdin);
    free(buf);
    fclose(s2m_ptr);
    fclose(result);
    freeSlaves(slaves);
    closeShm(shm, true);
    return 0;
}