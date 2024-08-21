#include <stdio.h>
#include <stdlib.h>
#include <log.h>

void err_msg(char *msg)
{
  if (msg != NULL) {
    fprintf(stderr, "%s\n", msg);
  }
}

void die(const int status, char *msg)
{
  if (status && msg != NULL) { // Non-zero status and non-empty message
    err_msg(msg);
  } else if (msg != NULL) { // Status is zero and non-empty message
    printf("%s\n", msg);
  }

  exit(status);
}

// vim:ts=4:sts=4:sw=4:et:ai:si:sta:noci:nopi:
