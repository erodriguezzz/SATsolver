// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/**
 * @file: posshmADT.c
 * @authors: Ezequiel Rodriguez, Juan I. Garcia M. & Jerónimo Brave.
 */

/* Local Includes */
#include "include/shmADT.h"

/* Standard & Sys lib's */
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>

int main(int argc, char const *argv[]) {
    int files = 0;
    char buffer[PIPE_BUF];
    shmADT shared;
    if (argc != 3) {
        char * shm_name = NULL;
        char * sem_name = NULL;
        size_t len=0;

        if ((len = getline(&shm_name, &len, stdin)) <= 0) return -1;

        shm_name[len-1] = 0;

        if ((len = getline(&sem_name, &len, stdin)) <= 0) return -1;

        sem_name[len-1] = 0;

        shared = newShm(shm_name, sem_name, O_RDWR, 0);
        
        free(shm_name); free(sem_name);
    } else {
        shared = newShm(argv[1], argv[2], O_RDWR, 0);
    }

    if (shared == NULL)
        return -1;  // TODO: Better handle error exits.
    
    int qty = readShm(shared, buffer, PIPE_BUF);
    if(qty == -1)
        return -1;
    files = atoi(buffer);

    while (files--) {
        if (readShm(shared, buffer, PIPE_BUF) == -1) {
            return -1;
        }
        printf("%s", buffer);
    }

    closeShm(shared, false);
    return 0;
}
