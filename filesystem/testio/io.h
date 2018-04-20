
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

int test_write(char *path, int flags, char *data, int len);
int test_direct_write(char *path);
int test_non_direct_write(char *path);
