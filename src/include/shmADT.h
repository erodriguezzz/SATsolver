/**
 * @file: shmADT.h
 * @author: Ezequiel Rodriguez, Juan I. Garcia M. & Jerónimo Brave.
 *
 * This ADT implements the System V IPC API or POSIX IPC API, depending
 * with which ??shmADT.c you compile, for communicating between processes
 * via share memory. Each instance of shmADT has a MAX_LENGTH buffer to write
 * to.
 *
 * Upon error errno is set appropriately.
 */

#ifndef SHM_ADT_H
#define SHM_ADT_H

#include <fcntl.h>
#include <stdbool.h>
#include <sys/types.h>

#define MAX_LENGTH 65536 // Shared memory buffer length
#define MAX_NAME_LENGTH 20
#define WRITE_SEM_NAME "noseevaluacalidaddecodigoenlasdefensas"

typedef struct shmCDT *shmADT;

/**
 * Creates and opens a new, or opens an existing, shared memory object and
 * semaphore to interact with the shm. Configured only for one reader and one
 * writer.
 *
 * @param[in] shm_name The share memory name at least 8 bytes long.
 * @param[in] sem_name The semaphore name at least 8 bytes long.
 * @param[in] flags The API flags for creation (O_CREAT) and permissions (O_RDWR
 * or O_RDONLY)
 * @param[in] mode The permissions as declared in <fcntl.h>.
 *
 * @return The share memory reference ADT or NULL in case of an error.
 */
shmADT newShm(const char *shm_name, const char *sem_name, int flags, int mode);

/**
 * Reads from the share memory count bytes or until '\0' is found,
 * which ever happens first. These are left in buf.
 *
 * @param[in] share The share memory reference ADT.
 * @param[out] buf The buffer in which the response is left.
 * @param[in] count The amount to be read.
 *
 * @return The amount read or -1 in case of an error.
 */
ssize_t readShm(shmADT share, char *buf, size_t count);

/**
 * Writes from the buf count bytes or until '\0' is found,
 * which ever happens first to the Share Memory.
 *
 * @param[in] share The share memory reference ADT.
 * @param[in] buf The buffer to be read.
 * @param[in] count The amount to be written.
 *
 * @return The amount written or -1 in case of an error.
 */
ssize_t writeShm(shmADT shm, const char *buf, size_t count);

/**
 * Closes the share memory reference, when all references are closed
 * it destroys it.
 *
 * @param[in] share The share memory reference ADT.
 * @param[in] creator If the user running the command created the share mem.
 *
 * @return 0 if all goes well or -1 in case of an error.
 */
int closeShm(shmADT share, bool creator);

#endif // SHM_ADT_H
