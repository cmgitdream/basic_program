
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

void usage()
{
  printf("./fcntl <file> <seconds>\n");
}

int flock(char *path, int type, int seconds)
{
  int r, fd = 0;
  fd = open(path, O_RDWR, 0644);
  if (fd < 0) {
    printf("%s open failed, error %s\n", path, strerror(errno));
    return fd;
  }
  printf("%s: fd = %d\n", __func__, fd);

  struct flock flock, flock2;
  memset(&flock, 0, sizeof(flock));
  flock.l_whence = SEEK_SET;
  // the whole file
  flock.l_start = 0;
  flock.l_len = 0;
  flock.l_type = type;

  flock2.l_whence = SEEK_SET;
  flock2.l_start = 0;
  flock2.l_len = 0;
  flock2.l_type = F_UNLCK;

  printf("fd = %d: will acquire lock\n", fd);

  int i = 0, n = 2;
  r = fcntl(fd, F_SETLK, &flock);
  if (r < 0) {
    printf("i=%d: flock set lock failed with type = %d\n", i, type);
    close(fd);
    return r;
  }
  printf("fd = %d: flock LOCK with type %d success\n", fd, type);
  printf("sleep %d seconds ...\n", seconds);
  sleep(seconds);
  
  printf("fd = %d: will release lock\n", fd);
  r = fcntl(fd, F_SETLK, &flock2);
  if (r < 0) {
    printf("i=%d: flock set unlock failed with type = %d\n", i, type);
    close(fd);
    return r;
  }
  printf("fd = %d: flock UNLOCK success\n", fd);
  printf("sleep %d seconds ...\n", seconds);
  sleep(seconds);
  //printf("fd = %d: close fd\n", fd);
  //close(fd); 
  return 0;
}

int main(int argc, char *argv[1])
{
  if (argc < 3) {
    usage();
    return -1;
  }
  int r = 0;
  int type = 0;
  r = flock(argv[1], F_WRLCK, atoi(argv[2]));
  return r;
}
