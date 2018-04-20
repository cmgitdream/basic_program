
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "io.h"

int test_write(char *path, int flags, char *data, int len)
{
  struct timeval tv1, tv2, tv3, tv4;
  gettimeofday(&tv1, NULL);
  int fd = open(path, flags, 0666);
  if (fd < 0) {
    printf("open %s failed\n", path);
    return -1;
  }
  gettimeofday(&tv2, NULL);
  ssize_t wsize = write(fd, data, len);
  gettimeofday(&tv3, NULL);
  if (close(fd) < 0) {
    printf("close file failed, error = %d\n", errno);
    return -errno;
  }
  gettimeofday(&tv4, NULL);

  printf(" open time elapsed:\t%lu second\t%lu microsecond\n", tv2.tv_sec - tv1.tv_sec, tv2.tv_usec - tv1.tv_usec);
  printf(" write time elapsed:\t%lu second\t%lu microsecond\n", tv3.tv_sec - tv2.tv_sec, tv3.tv_usec - tv2.tv_usec);
  printf(" close time elapsed:\t%lu second\t%lu microsecond\n", tv4.tv_sec - tv3.tv_sec, tv4.tv_usec - tv3.tv_usec);
  return 0;
}

int test_direct_write(char *path)
{
  char data[255] = "direct write string";
  int flags = O_CREAT|O_RDWR|O_DIRECT|O_SYNC;
  int len = strlen(data);
  printf("test DIRECT IO\n");
  return test_write(path, flags, data, len);
}

int test_non_direct_write(char *path)
{
  char data[255] = "non-direct w string";
  int flags = O_CREAT|O_RDWR;
  int len = strlen(data);
  printf("test non-DIRECT IO\n");
  return test_write(path, flags, data, len);
}
