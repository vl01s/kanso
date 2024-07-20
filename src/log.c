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
  // Non-zero status and non-empty message
  if (status && msg != NULL) {
    err_msg(msg);
  // Status is zero and non-empty message
  } else if (msg != NULL) {
    printf("%s\n", msg);
  }

  exit(status);
}
