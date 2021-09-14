// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/**
 * @file: posshmADT.c
 * @authors: Ezequiel Rodriguez, Juan I. Garcia M. & Jerónimo Brave.
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PATH_LENGTH 256
#define CLEAN_BUFF while (getchar() != '\n');

int main() {
  char buff[PIPE_BUF] = {0}, output[PIPE_BUF], path[MAX_PATH_LENGTH];
  char fmt[MAX_PATH_LENGTH];
  sprintf(fmt, "%%%ds", MAX_PATH_LENGTH);
  while (scanf(fmt, path) != EOF) {
    sprintf(output, "%d\tFILENAME: ", getpid());
    CLEAN_BUFF
    strcat(output, path);
    strcat(output, ":\t");
    FILE *stream = NULL;
    if (access(path, R_OK) == 0) {
      if (snprintf(buff, PIPE_BUF,
                   "minisat %s | grep -o -e \"Number of .*[0-9]\\+\" -e \"CPU "
                   "time.*\" -e \".*SATISFIABLE\" | tr '\\n' '\\t'",
                   path) > 0) {
        stream = popen(buff, "r");
      } else {
        perror("Failure in function access()");
        exit(1);
      }
      while (fgets(buff, PIPE_BUF, stream) != NULL) {
        strcat(output, buff);
      }
      pclose(stream);
    } else {
      strcat(output, "ERROR: Cannot access file.");
    }
    printf("%s\n", output);
    fflush(stdout);
  }

  return 0;
}
