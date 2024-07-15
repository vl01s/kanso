#include <log.h>
#include <stdio.h>

void err_msg(char *msg)
{
  if (msg != NULL) {
    fprintf(stderr, "%s\n", msg);
  }
}
