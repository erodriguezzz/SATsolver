// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/**
 * @file: svshmADT.c
 * @author: Ezequiel Rodriguez, Juan I. Garcia M. & Jerónimo Brave.
 */

#include "../include/shmADT.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

typedef struct t_shm * Pt_shm;

typedef struct t_shm {
    int w_pointer;
    int r_pointer;
    char buf[MAX_LENGTH];
} t_shm;

typedef struct shmCDT {
    Pt_shm shm;
    sem_t * sem;
    int shm_id;
    char sem_name[MAX_NAME_LENGTH];
} shmCDT;


shmADT newShm(const char * shm_name, const char * sem_name, int perms, int mode) {
    int key = (int) (*shm_name);
    shmADT shm_adt = malloc(sizeof(shmCDT));
    if (shm_adt == NULL) {
        return NULL;
    }

    strcpy(shm_adt->sem_name, sem_name);

    shm_adt->shm_id = shmget(key, sizeof(t_shm), perms);
    if (shm_adt->shm_id == -1) {
        return NULL;
    }
    
    shm_adt->shm = shmat(shm_adt->shm_id, NULL, S_IWUSR & perms ? 0 : SHM_RDONLY);
    if (shm_adt->shm == (void * ) -1) {
        return NULL;
    }

    if (perms & IPC_CREAT) {
        shm_adt->sem = sem_open(sem_name, O_CREAT, mode , 0);
        if (shm_adt->sem == SEM_FAILED) {
            return NULL;
        }
    } else {
        shm_adt->sem = sem_open(sem_name, 0);
        if (shm_adt->sem == SEM_FAILED) {
            return NULL;
        }
    }

    return shm_adt;
}

ssize_t readShm(shmADT share, char * buf, size_t count) {
    if (sem_wait(share->sem) == -1) {
        return -1;
    }
    
    ssize_t i = 0;
    while (share->shm->r_pointer < share->shm->w_pointer && i < count && share->shm->buf[share->shm->r_pointer] != '\0') {
        buf[i++] = share->shm->buf[share->shm->r_pointer++];
    }


    if (share->shm->w_pointer == 0) { // TODO: Check this.
        share->shm->r_pointer = 0;
    } else if (share->shm->buf[share->shm->r_pointer] == '\0') {
        share->shm->r_pointer++;
    }

    return i;
}

ssize_t writeShm(shmADT share, const char * buf, size_t count) {
    ssize_t i = 0;
    while (share->shm->w_pointer < MAX_LENGTH && i < count && buf[i] != '\0') {
        share->shm->buf[share->shm->w_pointer++] = buf[i++];
    }

    if (share->shm->w_pointer < MAX_LENGTH)
        share->shm->buf[share->shm->w_pointer++] = '\0';

    if (share->shm->w_pointer == MAX_LENGTH) {  // TODO: This can be avoided with a set buffer length of 2^N 
                                              // and a circular buffer with 2^N sized w & r pointers.
        share->shm->w_pointer = 0;
        errno = ENOMEM;
        return -1;
    }

    if (sem_post(share->sem) == -1) {
        return -1;
    }

    return i;
}

int closeShm(shmADT share, bool creator) {
    if(shmdt(share->shm) == -1)
        return -1;
    
    if (creator) {
        if(sem_unlink(share->sem_name) == -1)
            return -1;
        if (shmctl(share->shm_id, IPC_RMID, 0) == -1)
            return -1; 
    }

    if(sem_close(share->sem) == -1)
        return -1;

    free(share);
    return 0;
}
