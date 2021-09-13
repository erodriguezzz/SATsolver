/* Local Includes */
#include "include/shmADT.h"

/* Standard lib's */
#include <stdio.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <ctype.h>

/**
 * Translates a number represented by a string to a integer
 * e.g.: "123" -> (int) 123
 *
 * @param[in] str The string containing the number.
 *
 * @return The represented Integer
 * */
int strToNum(const char * str);

int main(int argc, char const *argv[]) {
    int files = 0;
    char buffer[MAX_LENGTH];
    shmADT shared  = newShm("somos unos kpo", "somos unos msckpo", O_RDWR, 0);
    if (shared == NULL)
        return -1;  // TODO: Better handle error exits.
    
    if (argc == 2) {  // TODO: Discuss how we can handle (int) files
        files = strToNum(argv[1]);
    } else if (argc == 1) {
        int qty = readShm(shared, buffer, MAX_LENGTH);
        if(qty == 0)
            return -1;
        files = strToNum(buffer);
    } else {
        return -1;
    }  
        
    while (files--) {
        if (readShm(shared, buffer, MAX_LENGTH) == -1) {
            return -1;
        }
        puts(buffer); // TODO: Analyze how we write to stdout.
    }

    closeShm(shared, false);
    return 0;
}

int strToNum(const char * str) {
    int result;
    while (*str && isdigit(*str))
        result = result * 10 + *(str++) - '0';
    return result;
}
