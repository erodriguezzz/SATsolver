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

int strtonum(const char * str){
    int i = 0, result;
    while(str[i]){
        result = result * 10 + str[i];
    }
    return result;
}

int main(int argc, char const *argv[])
{
    int files = 0;
    char buffer[MAX_LENGTH];
    shmADT shared  = newShm("somos unos kpo", "somos unos msckpo", O_RDWR, 0);
    if(shared == NULL)
        return -1;
    
    if(argc == 2){
        files = strtonum(argv[1]);
    } else if(argc == 1){
        int qty = readShm(shared, buffer, MAX_LENGTH);
        if(qty == 0)
            return -1;
        files = strtonum(buffer);
    } else {
        return -1;
    }  
        
    while(files--){
        if(readShm(shared, buffer, MAX_LENGTH) == -1){
            printf("Error leyendo buffer: %s\n", buffer);
            return -1;
        }
        printf("%s\n", buffer);
    }

    closeShm(shared, false);
}