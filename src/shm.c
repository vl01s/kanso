/* UNIX shm operations */

#include <errno.h>
#include <fcntl.h>
#include <log.h>
#include <shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

static void
randName(char name[], const int n)
{
  if (!name || n <= 0) {
    return;
  }

  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  long r = ts.tv_nsec;
  for (int i = 0; i < n; ++i) {
    name[i] =
      'A' + (r & 15) + (r & 16) * 2; // random letter from A to P or a to p
    r >>= 5;
  }
}

static int
createShm(void)
{
  char name[] = "/wl_shm-XXXXXX";
  /// TODO(DrKJeff16): I am not entirely comfortable with the `sizeof()`
  /// approach I'd suggest using a pointer array instead
  randName(name + sizeof(name) - 7, 6);
  int retries = 100, fd = -1;

  do {
    --retries;
    /// Opening in mode 644
    fd = shm_open(
      name, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  } while (fd < 0 && retries > 0 && errno == EEXIST);

  if (!retries) {
    verr("(createShm): %s\n", " Exceeded number of shm_open calls");
    return -1;
  }
  if (fd < 0) {
    verr("(createShm): %s (fd: %d)\n", "File descriptor invalid", fd);
    return -1;
  }

  shm_unlink(name);
  return fd;
}

int
allocateShm(const size_t size)
{
  int fd = -1;
  if ((fd = createShm()) < 0) {
    verr("(allocateShm): %s\n", "OS could not create an shm object");
    return -1;
  }

  int ret = -1;
  do {
    ret = ftruncate(fd, (off_t)size);
  } while (ret < 0 && errno == EINTR);

  if (ret < 0) {
    verr("(allocateShm): %s (fd: %d)\n",
         "OS could not allocate memory to an shm object",
         ret);
    return -1;
  }

  return fd;
}

// vim:ts=4:sts=4:sw=4:et:ai:si:sta:noci:nopi:
