#include <stdio.h>
#include <stdlib.h>
#include <log.h>

/**
 * Outputs string to STDERR
 *
 * @param msg Message string to print
 */
void err_msg(char *msg)
{
  if (msg != NULL) {
    fprintf(stderr, "%s\n", msg);
  }
}

/**
 * Kills the program execution
 *
 * If status is non-zero and message is given then output will be STDERR
 *
 * @param status Exit code for the program
 * @param msg Message string to print, can be `NULL` if no message is needed
 */
void die(const int status, char *msg)
{
  if (status && msg != NULL) { // Non-zero status and non-empty message
    err_msg(msg);
  } else if (msg != NULL) { // Status is zero and non-empty message
    printf("%s\n", msg);
  }

  exit(status);
}
