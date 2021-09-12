/**
 * @file: svshmADT.h
 * @author: Ezequiel Rodriguez, Juan I. Garcia M. & Jerónimo Brave.
 *
 * This TAD implements the System V IPC API and POSIX IPC API
 * for communicating between processes via share memory. Each instance
 * of shmADT has a MAX_LENGTH buffer to write to.
 */

#ifndef MV_SHM_ADT_H
#define MV_SHM_ADT_H

#include <fcntl.h>
#include <sys/types.h>
#include <stdbool.h>

#define MAX_LENGTH 1024 // TODO: Check how we can adapt this to suit the amount of files.

#define SEM_NAME "shm sem" // TODO: See how we can comunicate this.
#define MAX_NAME_LENGTH 20

/**
 * In case of an System V API error, errno will be set to
 *
 * */
#define ESHMGET 0x123
#define ESHMAT 0x124

typedef struct shmCDT * shmADT;

shmADT newShm(const char * shm_name, const char * sem_name, int flags, int mode);

ssize_t readShm(shmADT shm, char * buf, size_t count);

ssize_t writeShm(shmADT shm, const char * buf, size_t count);

int closeShm(shmADT shm, bool creator);

#endif // MV_SHM_ADT_H
