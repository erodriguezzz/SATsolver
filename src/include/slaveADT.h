/**
 * @file: slaveADT.h
 * @author: Ezequiel Rodriguez, Juan I. Garcia M. & Jerónimo Brave.
 *
 * This ADT implements a structure to store the slave processes and its
 * corresponding file descriptors.
 *
 * Upon error errno is set appropriately.
 */

#ifndef SLAVE_ADT_H
#define SLAVE_ADT_H

#include <unistd.h>
typedef struct slavesCDT * slavesADT;

/**
 * Creates the collection in which the slave information will be stored.
 * 
 * @return Returns NULL if it was not able to allocate memory. Else a
 * pointer to said structure is returned.
 */
slavesADT newSlaves();

/**
 * Adds a slave to the collection.
 * 
 * @param[in] slaves The collection to which the slave will be added.
 * @param[in] pid  Slave pid.
 * @param[in] fd_write File descriptor to the corresponding writing
 * end of the slave.
 * 
 * @return Returns 1 if the slave was successfully added, 0 otherwise.
 */
int addSlave(slavesADT slaves, int pid, int fd_write);

/**
 * Removes a slave from the collection.
 *
 * @param[in] slave_id Slave pid.
 * @return Returns 1 if the slave was successfully added, 0 otherwise.
 */
int rmSlave(slavesADT slaves, int slave_id);

/**
 * Returns the file descriptor of a slave.
 *
 * @param[in] slaves Slave collection.
 * @param[in] slave_id Slave pid.
 *
 * @return Returns -1 if the slave was not found within the collection.
 */
int getWriteFD(slavesADT slaves, int slave_id);

/**
 * Frees al allocated resources.
 *
 * @param[in] slaves Collection to be freed.
 */
void freeSlaves(slavesADT slaves);


#endif