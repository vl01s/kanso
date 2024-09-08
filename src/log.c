#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <log.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <operators.h>

void err(char *const msg)
{
    if (null_ptr(msg)) {
        return;
    }

    fprintf(stderr, "%s\n", msg);
}

int fd_log(int fd, char *const msg)
{
    if (fd < 0 || null_ptr(msg)) {
        return -1;
    }

    if (!strlen(msg)) {
        return -1;
    }

    return write(fd, msg, strlen(msg));
}

int fd_vlog(int fd, char *const fmt, ...)
{
    if (fd < 0 || null_ptr(fmt)) {
        return -1;
    }

    va_list argp;

    va_start(argp, fmt);
    int res = vdprintf(fd, fmt, argp);
    va_end(argp);

    return res;
}

int file_log(char *const fpath, char *const msg)
{
    if (null_ptr(fpath) || null_ptr(msg)) {
        return -1;
    }

    int fd = open(fpath, O_CREAT | O_APPEND, S_IROTH | S_IRUSR | S_IRGRP | S_IWUSR);

    if (fd < 0) {
        verr("(file_log): File `%s` can't be opened (fd: %d)", fpath, fd);
        return -1;
    }

    int write_d = write(fd, msg, strlen(msg));
    if (write_d < 0) {
        verr("(file_log): File `%s` couldn't be written to (fd: %d)", fpath, fd);
        close(fd);
        return write_d;
    }

    return close(fd);
}

int file_vlog(char *const fpath, char *const fmt, ...)
{
    if (null_ptr(fpath) || null_ptr(fmt)) {
        return -1;
    }

    int fd = open(fpath, O_CREAT | O_APPEND, S_IROTH | S_IRUSR | S_IRGRP | S_IWUSR);

    if (fd < 0) {
        verr("(file_vlog): File `%s` can't be opened (fd: %d)", fpath, fd);
        return -1;
    }

    va_list argp;

    va_start(argp, fmt);
    int res = vdprintf(fd, fmt, argp);
    va_end(argp);

    if (res > 0) {
        return close(fd);
    }

    /// If `res` is less than 0
    return res;
}

void verr(char *const fmt, ...)
{
    if (null_ptr(fmt)) {
        return;
    }

    va_list argp;

    va_start(argp, fmt);
    vfprintf(stderr, fmt, argp);
    va_end(argp);
}

void die(const int status, char *const msg)
{
    if (!null_ptr(msg)) { /// Non-empty message
        fprintf(status ? stderr : stdout, "%s\n", msg);
    }

    exit(status);
}

void vdie(const int status, char *const fmt, ...)
{
    if (!null_ptr(fmt)) { /// Non-empty format string
        va_list argp;

        va_start(argp, fmt);
        vfprintf(status ? stderr : stdout, fmt, argp);
        va_end(argp);
    }

    exit(status);
}

// vim:ts=4:sts=4:sw=4:et:ai:si:sta:noci:nopi:
