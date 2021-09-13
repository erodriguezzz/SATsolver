/* Local Includes */
#include "include/shmADT.h"

/* Standard lib's */
#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_NAME_LEN 20

/**
 * Translates a number represented by a string to a integer
 * e.g.: "123" -> (int) 123
 *
 * @param[in] str The string containing the number.
 *
 * @return The represented Integer
 * */
int strToNum(const char * str);

int main(int argc, char const *argv[]) {
    int files = 0;
    char buffer[MAX_LENGTH];
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
    
    int qty = readShm(shared, buffer, MAX_LENGTH);
    if(qty == -1)
        return -1;
    files = atoi(buffer);

    while (files--) {
        if (readShm(shared, buffer, MAX_LENGTH) == -1) {
            return -1;
        }
        printf("%s", buffer); // TODO: Analyze how we write to stdout.
    }

    closeShm(shared, false);
    return 0;
}
