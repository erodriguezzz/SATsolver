/**
 * @file: svshmADT.c
 * @author: Ezequiel Rodriguez, Juan I. Garcia M. & Jerónimo Brave.
 */

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

typedef struct t_shm * Pt_shm;

typedef struct t_shm {
    int w_pointer;
    int r_pointer;
    char buf[MAX_LENGTH];
} t_shm;

typedef struct shmCDT {
    Pt_shm shm;  // TODO: Change to a better var name.
    sem_t * sem;
    int shm_id;
} shmCDT;


shmADT newShm(key_t key, int perms) {
    shmADT shm_adt = malloc(sizeof(shmCDT));
    if (shm_adt == NULL) {
        return NULL;
    }

    shm_adt->shm_id = shmget(key, sizeof(t_shm), perms);
    if (shm_adt->shm_id == -1) {
        errno = ESHMGET;
        return NULL;
    }
    
    shm_adt->shm = shmat(shm_adt->shm_id, NULL, S_IWUSR & perms ? 0 : SHM_RDONLY);
    if (shm_adt->shm == (void * ) -1) {
        errno = ESHMAT;
        return NULL;
    }

    if (perms & IPC_CREAT) {
        shm_adt->sem = sem_open(SEM_NAME, O_CREAT, 0644 , 0); // TODO: Check perms (0644).
        if (shm_adt->sem == SEM_FAILED) {
            return NULL;
        }
    } else {
        shm_adt->sem = sem_open(SEM_NAME, 0); // TODO: Check how we pass the SEM_NAME.
        if (shm_adt->sem == SEM_FAILED) {
            return NULL;
        }
    }

    return shm_adt;
}

ssize_t readShm(shmADT shm, char * buf, size_t count) {
    if (sem_wait(shm->sem) == -1) {
        return -1;
    }
    
    ssize_t i = 0;
    while (shm->shm->r_pointer < shm->shm->w_pointer && i < count && shm->shm->buf[shm->shm->r_pointer] != '\0') { 
        buf[i++] = shm->shm->buf[shm->shm->r_pointer++];
    }


    if (shm->shm->w_pointer == 0) { // TODO: Check this.
        shm->shm->r_pointer = 0;
    } else if (shm->shm->buf[shm->shm->r_pointer] == '\0') {
        shm->shm->r_pointer++;
    }

    // if (i != count) {
    //     buf[i]='\0';
    // }

    return i;
}

ssize_t writeShm(shmADT shm, const char * buf, size_t count) {
    ssize_t i = 0;
    while (shm->shm->w_pointer < MAX_LENGTH && i < count && buf[i] != '\0') {
        shm->shm->buf[shm->shm->w_pointer++] = buf[i++];
    }

    if (shm->shm->w_pointer < MAX_LENGTH)
        shm->shm->buf[shm->shm->w_pointer++] = '\0';

    if (shm->shm->w_pointer == MAX_LENGTH) {  // TODO: This can be avoided with a set buffer length of 2^N 
                                              // and a circular buffer with 2^N sized w & r pointers.
        shm->shm->w_pointer = 0;
        errno = ENOMEM;
        return -1;
    }

    if (sem_post(shm->sem) == -1) {
        return -1;
    }

    return i;
}

int closeShm(shmADT shm, bool creator) {
    if(shmdt(shm->shm) == -1)
        return -1;
    
    if (creator) {
        if(sem_unlink(SEM_NAME) == -1)
            return -1;
        if (shmctl(shm->shm_id, IPC_RMID, 0) == -1)
            return -1; 
    }

    if(sem_close(shm->sem) == -1)
        return -1;

    free(shm);
}
