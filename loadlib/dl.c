
#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>

void usage()
{
	printf("./dl /path/to/so\n");
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		usage();
		exit(-1);
	}
	void *handle = NULL;
	char *sofile = argv[1]; //"/usr/lib/ceph/compressor/libceph_snappy.so";
	char *sofile2 = "/usr/lib/ceph/erasure-code/libec_jerasure.so";
	/*
	if ((handle = dlopen(sofile2, RTLD_NOW)) == NULL) {
		printf("dlopen %s failed %s\n", sofile2, dlerror());
		exit(-1);
	}
	*/
	if ((handle = dlopen(sofile, RTLD_NOW)) == NULL) {
		printf("dlopen %s failed %s\n", sofile, dlerror());
		exit(-1);
	}
	printf(" dlopen success\n");
	return 0;
}
