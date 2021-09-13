/**
 * @file pipesADT.h
 * ADT to manage different file descriptors from slave processes.
 * NOTE: collection works best if slave_id are consecutive numbers
 * starting from zero.
 *
 */

#ifndef SLAVE_ADT_H
#define SLAVE_ADT_H

#include <unistd.h>
typedef struct slavesCDT * slavesADT;

/**
 * Creates empty slave collection.
 * @return Returns NULL if it wasn't able to allocate memory.
 */
slavesADT newSlaves();

/**
 * Adds a slave to the collection.
 * @return Returns 1 if slave was successfully added, 0 otherwise.
 */
int addSlave(slavesADT slaves, int slave_id, int fd_read, int fd_write);

/**
 * Removes a slave from the collection.
 * @return Returns 1 if slave was successfully deleted, 0 otherwise.
 */
int rmSlave(slavesADT slaves, int slave_id);

/**
 * Indicates which slaves are ready for read.
 * @param[out] available_ids
 * @return Returns the dimension of array available_ids[].
 * Upon error, returns -1.
 *
 */
size_t available(slavesADT slaves, int available_ids[]);

int getWriteFD(slavesADT slaves, int slave_id);

int getReadFD(slavesADT slaves, int slave_id);

/**
 * Frees all allocated resources.
 * @param slaves
 */
void freeSlaves(slavesADT slaves); // \( ﾟヮﾟ)/

#endif