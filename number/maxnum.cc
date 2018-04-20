
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <inttypes.h>

int main(int argc, char *argv[])
{
  int64_t maxnum = 0x7FFFFFFFFFFFFFFF;
  int64_t maxblocks = maxnum >> 10;
  printf("uint64_t(-1): %llu\n", -1);
  printf("maxnum: %llu\n", maxnum);
  printf("maxblocks: %llu %llX\n", maxblocks, maxblocks);
  return 0;
}
