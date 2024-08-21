#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <log.h>

void err_msg(char *msg)
{
    if (!msg || msg == NULL) {
        return;
    }

    fprintf(stderr, "%s\n", msg);
}

void verr_msg(char *const fmt, ...)
{
    if (!fmt || fmt == NULL) {
        return;
    }

    va_list argp;

    va_start(argp, fmt);
    vfprintf(stderr, fmt, argp);
    va_end(argp);
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

void vdie(const int status, char *const fmt, ...)
{
    if (fmt && fmt != NULL) {
        FILE *f = stdout;
        va_list argp;

        if (status) {
            f = stderr;
        }

        va_start(argp, fmt);
        vfprintf(f, fmt, argp);
        va_end(argp);
    }

    exit(status);
}

// vim:ts=4:sts=4:sw=4:et:ai:si:sta:noci:nopi:
