#ifndef KANSO_SHM_H
#define KANSO_SHM_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>

static void randname(char[], const int);
static int createShm(void);
int allocateShm(const size_t);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !KANSO_SHM_H
