#ifndef SLAVE_ADT_H
#define SLAVE_ADT_H

#include <unistd.h>
typedef struct slavesCDT * slavesADT;

slavesADT newSlaves();

int addSlave(slavesADT slaves, int pid, int fd_write);

int rmSlave(slavesADT slaves, int slave_id);

int getWriteFD(slavesADT slaves, int slave_id);

void freeSlaves(slavesADT slaves);


#endif