#include "include/slaveADT.h"
#include <stdlib.h>
#include <sys/select.h>


typedef struct slave{
    char occupied;
    int fd_read;
    int fd_write;
}slave;

typedef struct slavesCDT{
    slave *slaves;
    size_t amount, largest;
    fd_set *active_set;
} slavesCDT;

slavesADT newSlaves(){
    slavesADT new = malloc(sizeof(slavesCDT));
    if(new == NULL)
        return NULL;
    new->slaves = NULL;
    new->largest = 0;
    new->active_set = malloc(sizeof(fd_set));
    if(new->active_set == NULL)
        return NULL;
    FD_ZERO(new->active_set);
    return new;

}

int addSlave(slavesADT slaves, int slave_id, int fd_read, int fd_write){
    if(slaves == NULL)
        return 0;
    if(slave_id + 1 >= slaves->amount){
        slaves->slaves = realloc(slaves->slaves, (slave_id + 1) * sizeof(slave));
        if(slaves->slaves == NULL)
            return 0;
        for(size_t i = slaves->amount; i <= slave_id; i++)
            slaves->slaves[i].occupied = 0;
        slaves->amount = slave_id + 1;
    }
    slaves->slaves[slave_id].fd_read = fd_read;
    slaves->slaves[slave_id].fd_write = fd_write;
    slaves->slaves[slave_id].occupied = 1;
    FD_SET(fd_read, slaves->active_set);
    if(fd_read >= slaves->largest)
        slaves->largest = fd_read;
    return 1;
}

int rmSlave(slavesADT slaves, int slave_id){
    if(slaves == NULL)
        return 0;
    if(slave_id >= slaves->amount)
        return 0;
    FD_CLR(slaves->slaves[slave_id].fd_read, slaves->active_set);
    slaves->slaves[slave_id].occupied = 0;
    return 1;
}

size_t available(slavesADT slaves, int *ready_ids){
    if(slaves == NULL)
        return -1;
    fd_set read_set = *slaves->active_set;
    if(select((int) slaves->largest + 1, &read_set, NULL, NULL, NULL) < 0){
        return -1;
    }
    int added = 0;
    for(int i = 0; i < slaves->amount; i++){
        if(slaves->slaves[i].occupied && FD_ISSET(slaves->slaves[i].fd_read, &read_set)){
            ready_ids[added++] = i;
        }
    }
    return added;
}

int getWriteFD(slavesADT slaves, int slave_id){
    if(slaves == NULL || slave_id >= slaves->amount || !slaves->slaves[slave_id].occupied)
        return -1;
    return slaves->slaves[slave_id].fd_write;
}

int getReadFD(slavesADT slaves, int slave_id){
    if(slaves == NULL || slave_id >= slaves->amount || !slaves->slaves[slave_id].occupied)
        return -1;
    return slaves->slaves[slave_id].fd_read;
}

void freeSlaves(slavesADT slaves){
    free(slaves->slaves);
    free(slaves->active_set);
    free(slaves);
}
