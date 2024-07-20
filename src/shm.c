/* UNIX shm operations */

#include <log.h>
#include <shm.h>

/* 'randName' generates a random name for the first 'n' characters of 'name'. */
void randName(char name[], const int n)
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
} // STATIC

int createShm(void)
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
} // STATIC

int allocateShm(const size_t size)
{
    int fd = createShm();
    if (fd < 0) {
        err_msg("OS could not create an shm object.");
        return -1;
    }
    int ret;
    do {
        ret = ftruncate(fd, size);
    } while (ret < 0 && errno == EINTR);
    if (ret < 0) {
        err_msg("OS could not allocate memory to an shm object.");
        return -1;
    }
    return fd;
}
