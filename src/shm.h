#ifndef KANSO_SHM_H
#define KANSO_SHM_H

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>

static void randname(char[], const int);
static int createShm(void);
int allocateShm(const size_t);

#endif // !KANSO_SHM_H
