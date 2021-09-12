#include <stdio.h>
#include "include/shmADT.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

#define OBJ_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

int main(void) {
    shmADT shm = newShm("somos unos kpo", "somos unos msckpo", O_RDWR | O_CREAT, 0777);
    if (shm == NULL) {
        puts("Cagaste pa.\n");
        return 1;
    }
    
    if (writeShm(shm, "Hola papa\n", 10000) == -1) {
        puts("Cagaste pa. 1\n");
        return 1;
    }
    sleep(3);
    if (closeShm(shm, true) == -1) {
        puts("Cagaste pa. 2\n");
        return 1;
    }
    return 0;
}