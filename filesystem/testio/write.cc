
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

void usage()
{
  printf("./write <file>\n");
}

int main(int argc, char *argv[])
{
  if (argc < 2) {
    usage();
    exit(-1);
  }

  test_non_direct_write(argv[1]);
}
