
#include <stdio.h>
#include <stdlib.h>

#define UINT64_MAX (18446744073709551615ULL)

int main(int argc, char *argv[])
{

  printf("%llX --> %ld\n", UINT64_MAX >> 11, UINT64_MAX >> 11);
  return 1;
}
