#ifndef KANSO_MEMORY_H
#define KANSO_MEMORY_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#ifndef MALLOC
#define MALLOC(type) (type*)malloc(sizeof(type))
#endif /* !MALLOC */

#ifndef CALLOC
#define CALLOC(type, n) (type*)calloc(n, sizeof(type))
#endif /* !CALLOC */

#ifndef REALLOC
#define REALLOC(ptr, type, n) (type*)realloc((void*)ptr, sizeof(type) * n)
#endif /* !REALLOC */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !KANSO_MEMORY_H */

/// vim:ts=4:sts=4:sw=4:et:ai:si:sta:noci:noet:ai:si:sta:noci:nopi:
