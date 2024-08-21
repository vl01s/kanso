#ifndef KANSO_SHM_H
#define KANSO_SHM_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <sys/types.h>

/**
 * Generates a random name for the first 'n' characters of 'name'
 *
 * @param name The string name
 * @param n The number of first `n` chars
 */
static void randname(char name[], const int n);
static int createShm(void);
int allocateShm(const size_t size);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !KANSO_SHM_H */

// vim:ts=4:sts=4:sw=4:et:ai:si:sta:noci:nopi:
