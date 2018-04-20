
#include <stdio.h>
#include <stdlib.h>

void usage()
{
  printf(" <str_num>\n");
}

int main(int argc, char *argv[])
{
  if (argc < 2) {
    usage();
    return -1;
  }
  printf("result: char %s --> %u\n", argv[1], strtoul(argv[1], NULL, 10));
  return 0;
}
