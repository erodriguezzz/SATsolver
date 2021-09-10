#define _DEFAULT_SOURCE 


#include <stdio.h>
#include <sys/ipc.h>
#include <stdlib.h>

#define FILES_PER_SLAVE 2

int main(int argc, char const *argv[]) {
    
    struct shmseg *pShm;

    if (argc == 1) {
        fprintf(stderr, "No files inputted. \n");
        exit(1);
    }

    int read_end_slaves[] = {};

    int slaves = (argc - (argc % 2 ? 0 : 1)) / FILES_PER_SLAVE;

    while (slaves != 0) {
        
    }
    

    // -------------------------------
    //https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_shared_memory.htm
    
    void * pShm = newShm(SHM_KEY, O_RDWR);
    
    
    return 0;
}
