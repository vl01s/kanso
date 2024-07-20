#ifndef KANSO_LOG_H
#define KANSO_LOG_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdio.h>
#include <stdlib.h>

void err_msg(char*);
void die(const int, char*);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !KANSO_LOG_H
