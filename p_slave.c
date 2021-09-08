#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//#define MAX_LENGTH 102

int main(int argc, const char *argv[]) {
    if (argc == 1) {
        return -1;
    }
    int p[2];
    while(--argc) {
        if(pipe(p) < 0)
            exit(1);
        if (fork() == 0) {
            close(1);
            dup(p[1]);
            close(p[0]);
            close(p[1]);
            execlp("/usr/bin/minisat", "minisat", argv[argc], (char *) NULL);
        }
        if(fork() == 0){
            close(0);
            dup(p[0]);
            close(p[0]);
            close(p[1]);
            execlp("/bin/grep","grep", "-o", "-e \"Number of .*[0-9]\\+\"", "-e \"CPU time.*\"", "-e \".*SATISFIABLE\"", (char *) NULL);
        }
        close(p[0]);
        close(p[1]);
        //wait()
        //wait()
            //char buffer[MAX_LENGTH] = {0};
            
            
            // FILE * cmd = popen("minisat", "r");
            
            
    }
        // minisat <archivo> | grep -o -e "Number of .*[0-9]\+" - e "CPU time.*" -e ".*SATISFIABLE"
        // , "| grep -o -e \"Number of .*[0-9]\\+\" - e \"CPU time.*\" -e \".*SATISFIABLE\""}//return execlp("/usr/bin/minisat", "minisat", argv[argc], (char *) NULL);
    return 0;
}