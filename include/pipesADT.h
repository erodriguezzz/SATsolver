#ifndef PIPES_ADT_H
#define PIPES_ADT_H

#include <unistd.h>
typedef struct slavesCDT * slavesADT;

slavesADT newSlaves();

int addSlave(slavesADT slaves, int slave_id, int fd_read, int fd_write);

int rmSlave(slavesADT slaves, int slave_id);

size_t available(slavesADT slaves, int *ready_ids);

int getWriteFD(slavesADT slaves, int slave_id);

int getReadFD(slavesADT slaves, int slave_id);

void freeSlaves(slavesADT slaves); // \( ﾟヮﾟ)/

#endif