#ifndef MV_SHM_LIB_H
#define MV_SHM_LIB_H

#include <limits.h>

#define BUF_SIZE PIPE_BUF
#define SHM_KEY 0x1234

#define O_RDONLY         00
#define O_WRONLY         01
#define O_RDWR           02

typedef struct shmCDT * shmADT;

shmADT newShm()


#endif // MV_SHM_LIB_H
