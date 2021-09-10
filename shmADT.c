#include "include/shm_lib.h"


// Defines structure of shared memory segment and semaphore state
typedef struct shmCDT {
    int cnt;
    char buf[BUF_SIZE];
    int semid;
    int shmid;
    int shmkey;
};

void * create_shm(){
    void * shmp;
    struct shmseg *shmp;

    //identificamos el segmento de memoria compartida a asignarle al proceso aplicación y vista
    int shmid = shmget(SHM_KEY, sizeof(), IPC_CREAT | OBJ_PERMS);
    if (shmid == -1)
        errExit("shmget");
    
    //Como los flags estan seteados en 0 y el proceso aplicacion tiene permisos de lectura y escritura
    // el segmento de sharememory tambien
    shmp = shmat(shmid, NULL, 0);
    if (shmp == (void *) -1)
        errExit("shmat");

    return shmp
}
