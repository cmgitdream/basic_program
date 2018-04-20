
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int r = 0;
  struct timespec ts;
  r = clock_gettime(CLOCK_REALTIME, &ts);
  if (r < 0) {
    printf("get time failed\n");
    exit(1);
  }
  printf("realtime %lu sec %lu nano sec\n", ts.tv_sec, ts.tv_nsec);
}
