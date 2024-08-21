/* UNIX shm operations */

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <log.h>
#include <shm.h>

static void randName(char name[], const int n)
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
    /// TODO(DrKJeff16): I am not entirely comfortable with the `sizeof()` approach
    /// I'd suggest using a pointer array instead
    randName(name + sizeof(name) - 7, 6);
    int retries = 100, fd = -1;

    do {
        --retries;
        /// Opening in mode 644
        fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    } while (fd < 0 && retries > 0 && errno == EEXIST);

    if (!retries && fd < 0) {
        err_msg("Exceeded number of shm_open calls");
        return -1;
    } else if (fd < 0) {
        return -1;
    }

    shm_unlink(name);
    return fd;
}

int allocateShm(const size_t size)
{
    int fd = -1;
    if ((fd = createShm()) < 0) {
        err_msg("OS could not create an shm object.");
        return -1;
    }

    int ret = -1;
    do {
        ret = ftruncate(fd, (off_t)size);
    } while (ret < 0 && errno == EINTR);

    if (ret < 0) {
        err_msg("OS could not allocate memory to an shm object.");
        return -1;
    }

    return fd;
}

// vim:ts=4:sts=4:sw=4:et:ai:si:sta:noci:nopi:
