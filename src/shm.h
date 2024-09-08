#ifndef KANSO_SHM_H
#define KANSO_SHM_H 1

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

#include <types.h>

/**
 * Generates a random name for the first 'n' characters of 'name'
 *
 * @param name The string name
 * @param n The number of first `n` chars
 */
static void randName(char name[], const int n);
static int createShm(void);
int allocateShm(const ko_size_t size);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* !KANSO_SHM_H */

// vim:ts=4:sts=4:sw=4:et:ai:si:sta:noci:nopi:
