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


int main(void) {
    shmADT shm  = newShm("somos unos kpo", "somos unos msckpo", O_RDONLY, 0);
    if(shm == NULL) {
        printf("Cagaste pa.\n");
        return 1;
    }
    char buffer[1000]={0};
    if (readShm(shm, buffer, 1000) == -1) {
        puts("Cagaste pa 1.\n");
        return 1;
    }
    printf("hola\n");
    if (closeShm(shm, false) == -1) {
        puts("cagaste pa 3.\n");
        return 1;
    }
    return 0;
}