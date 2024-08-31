#ifndef KANSO_MEMORY_H
#define KANSO_MEMORY_H 1

#if defined(__cplusplus)
extern "C"
{
#endif /* __cplusplus */

#include "types.h"

#ifndef MALLOC
#define MALLOC(type) KANSO_CAST(type*, malloc(sizeof(type)))
#endif /* !MALLOC */

#ifndef NMALLOC
#define NMALLOC(type, n) KANSO_CAST(type*, malloc(sizeof(type) * n))
#endif /* !NMALLOC */

#ifndef CALLOC
#define CALLOC(type, n) KANSO_CAST(type*, calloc(n, sizeof(type)))
#endif /* !CALLOC */

#ifndef REALLOC
#define REALLOC(ptr, type, n)                                                  \
  KANSO_CAST(type*, realloc(KANSO_CAST(void*, ptr), sizeof(type) * n))
#endif /* !REALLOC */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* !KANSO_MEMORY_H */

/// vim:ts=4:sts=4:sw=4:et:ai:si:sta:noci:nopi:
