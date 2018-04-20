#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


const char *data = "this is a test line";
char buf[32];

int create_file(const char *file, int flags)
{
  return open(file, flags|O_RDWR|O_CREAT, 0644);
}

int open_file(const char *file, int flags)
{
  return open(file, flags|O_RDWR, 0644);
}

int close_file(int fd)
{
  return close(fd);
}

void usage()
{
  printf("usage: create|open <file>\n");
}

int main(int argc, char *argv[])
{
  if (argc < 3) {
    usage();
    exit(-1);
  }

  int fd = -1;
  int fd2 = -1;
  int fd3 = -1;
  int fd4 = -1;
  if (strcmp(argv[1],"create") == 0) {
    // 1. open
    fd = create_file(argv[2], 0); 
    if (fd < 0) {
      printf(" create file failed, error = %d\n", -errno);
      return -EINVAL;
    }
    printf("fd = %d\n", fd);
    write(fd, data, sizeof(char) * strlen(data));

    // 2. open
    fd2 = open_file(argv[2], 0);
    if (fd2 < 0) {
      printf(" open file failed, error = %d\n", -errno);
      return -EINVAL;
    }
    printf("fd2 = %d\n", fd2);

    memset(buf, '\0', 32);
    read(fd2, buf, 32);
    printf("read data = %s\n", buf);

    // 3. close
    close_file(fd);

    // 4. open
    fd3 = open_file(argv[2], 0);
    if (fd3 < 0) {
      printf(" open file failed, error = %d\n", -errno);
      return -EINVAL;
    }
    printf("fd3 = %d\n", fd3);

    // 5. open
    fd4 = open_file(argv[2], 0);
    if (fd4 < 0) {
      printf(" open file failed, error = %d\n", -errno);
      return -EINVAL;
    }
    printf("fd4 = %d\n", fd4);
  } else if (strcmp(argv[1], "open") == 0) {
    fd = open_file(argv[2], 0);
    if (fd < 0) {
      printf(" open file failed, error = %d\n", -errno);
      return -EINVAL;
    }
    printf("fd = %d\n", fd);
    close_file(fd);
    int nfd = open_file(argv[2], 0);
    if (fd < 0) {
      printf(" open file failed, error = %d\n", -errno);
      close_file(fd);
      return -EINVAL;
    }
    printf("nfd = %d\n", nfd);
    close_file(nfd);
  } else {
    usage();
    exit(-1);
  }

}
