#ifndef KANSO_SHM_H
#define KANSO_SHM_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <sys/types.h>

static void randname(char[], const int);
static int createShm(void);
int allocateShm(const size_t);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !KANSO_SHM_H

// vim:ts=4:sts=4:sw=4:et:
