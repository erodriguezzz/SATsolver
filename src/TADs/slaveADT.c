#include "../include/slaveADT.h"
#include <stdlib.h>
#include <unistd.h>


typedef struct slave{
    char occupied;
    int fd_write;
    int pid;
}slave;

typedef struct slavesCDT{
    slave *slaves;
    size_t amount;
} slavesCDT;

slavesADT newSlaves(){
    slavesADT new = malloc(sizeof(slavesCDT));
    if(new == NULL)
        return NULL;
    new->slaves = NULL;
    new->amount = 0;
    return new;

}

int addSlave(slavesADT slaves, int pid, int fd_write){
    if(slaves == NULL)
        return 0;
    slaves->slaves = realloc(slaves->slaves, (slaves->amount + 1) * sizeof(slave));
    if(slaves->slaves == NULL)
        return 0;
    slaves->slaves[slaves->amount].pid = pid;
    slaves->slaves[slaves->amount].occupied = 1;
    slaves->slaves[slaves->amount++].fd_write = fd_write;

    return 1;
}

int rmSlave(slavesADT slaves, int slave_id){
    if(slaves == NULL)
        return 0;
    for(int i = 0; i < slaves->amount; i++){
        if(slaves->slaves[i].pid == slave_id){
            slaves->slaves[i].occupied = 0;
            return 1;
        }
    }
    return 0;
}

int getWriteFD(slavesADT slaves, int slave_id){
    if(slaves == NULL)
        return -1;
    for(int i = 0; i < slaves->amount; i++){
        if(slaves->slaves[i].occupied && slaves->slaves[i].pid == slave_id){
            return slaves->slaves[i].fd_write;
        }
    }
    return -1;
}


void freeSlaves(slavesADT slaves){
    free(slaves->slaves);
    free(slaves);
}
