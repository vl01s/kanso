#ifndef KANSO_LOG_H
#define KANSO_LOG_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * Outputs string to STDERR
 *
 * @param msg Message string to print
 */
void err_msg(char *const msg);
void verr_msg(char *const fmt, ...);

/**
 * Kills the program execution
 *
 * If status is non-zero and message is given then output will be STDERR
 *
 * @param status Exit code for the program
 * @param msg Message string to print, can be `NULL` if no message is needed
 */
void die(const int status, char *const msg);
void vdie(const int status, char *const fmt, ...);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !KANSO_LOG_H */

// vim:ts=4:sts=4:sw=4:et:ai:si:sta:noci:nopi:
