/* UNIX shm operations */

#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include "shm.h"

/* 'randName' generates a random name for the first 'n' characters of 'name'. */
static void randName(char name[], int n)
{
    if (name && n > 0) {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        long r = ts.tv_nsec;
        for (int i = 0; i < n; ++i) {
            name[i] = 'A' + (r & 15) + (r & 16) * 2; // random letter from A to P or a to p
            r >>= 5;
        }
    }
}

static int createShm(void)
{
    char name[] = "/wl_shm-XXXXXX";
    randName(name + sizeof(name) - 7, 6);
    int retries = 100;
    int fd;
    do {
        --retries;
        fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    } while (fd < 0 && retries > 0 && errno == EEXIST);
    if (fd < 0) {
        return -1;
    }
    shm_unlink(name);
    return fd;
}

int allocateShm(size_t size)
{
    int fd = createShm();
    if (fd < 0) {
        // TODO(vluis): Replace with propper log system
        fprintf(stderr, "OS could not create an shm object.\n");
        return -1;
    }
    int ret;
    do {
        ret = ftruncate(fd, size);
    } while (ret < 0 && errno == EINTR);
    if (ret < 0) {
        // TODO(vluis): Replace with propper log system
        fprintf(stderr, "OS could not allocate memory to an shm object.\n");
        return -1;
    }
    return fd;
}
