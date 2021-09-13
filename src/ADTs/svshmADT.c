// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/**
 * @file: svshmADT.c
 * @authors: Ezequiel Rodriguez, Juan I. Garcia M. & Jerónimo Brave.
 */

#include "../include/shmADT.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>
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


shmADT newShm(const char * shm_name, const char * sem_name, int flags, int mode) {
    int key = (int) (*shm_name);
    shmADT shm_adt = malloc(sizeof(shmCDT));
    if (shm_adt == NULL) {
        return NULL;
    }

    strcpy(shm_adt->sem_name, sem_name);

    shm_adt->shm_id = shmget(key, sizeof(t_shm), flags & O_CREAT ? IPC_CREAT | mode : 0);
    if (shm_adt->shm_id == -1) {
        free(shm_adt);
        return NULL;
    }

    shm_adt->shm = shmat(shm_adt->shm_id, NULL, O_RDWR & flags ? 0 : SHM_RDONLY);
    if (shm_adt->shm == (void * ) -1) {
        closeShm(shm_adt, flags & O_CREAT);
        return NULL;
    }

    if (flags & O_CREAT) {
        shm_adt->sem = sem_open(sem_name, O_CREAT, mode , 0);
        if (shm_adt->sem == SEM_FAILED) {
            closeShm(shm_adt, flags & O_CREAT);
            return NULL;
        }
    } else {
        shm_adt->sem = sem_open(sem_name, 0);
        if (shm_adt->sem == SEM_FAILED) {
            closeShm(shm_adt, flags & O_CREAT);
            return NULL;
        }
    }

    return shm_adt;
}

ssize_t readShm(shmADT shared, char * buf, size_t count) {
    if (sem_wait(shared->sem) == -1) {
        return -1;
    }

    ssize_t i = 0;
    while (shared->shm->r_pointer < shared->shm->w_pointer && i < count && shared->shm->buf[shared->shm->r_pointer] != '\0') {
        buf[i++] = shared->shm->buf[shared->shm->r_pointer++];
    }


    if (shared->shm->w_pointer == 0) { // This should not happen with a correctly sized MAX_LENGTH.
        shared->shm->r_pointer = 0;
    } else if (shared->shm->buf[shared->shm->r_pointer] == '\0') {
        shared->shm->r_pointer++;
    }

    if (i < count) {
        buf[i] = '\0';
    }

    return i;
}

ssize_t writeShm(shmADT shared, const char * buf, size_t count) {
    ssize_t i = 0;
    while (shared->shm->w_pointer < MAX_LENGTH && i < count && buf[i] != '\0') {
        shared->shm->buf[shared->shm->w_pointer++] = buf[i++];
    }

    if (shared->shm->w_pointer < MAX_LENGTH)
        shared->shm->buf[shared->shm->w_pointer++] = '\0';

    if (shared->shm->w_pointer == MAX_LENGTH) {  /* This can be avoided with a set buffer length of 2^N
                                                    and a circular buffer with 2^N sized w & r pointers. */
        shared->shm->w_pointer = 0;
        errno = ENOMEM;
        return -1;
    }

    if (sem_post(shared->sem) == -1) {
        return -1;
    }

    return i;
}

int closeShm(shmADT share, bool creator) {
    shmdt(share->shm);

    if (creator) {
        sem_unlink(share->sem_name);
        shmctl(share->shm_id, IPC_RMID, 0);
    }

    sem_close(share->sem);

    free(share);
    return 0;
}
