
#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>

int open_dl()
{
	void *handle = NULL;
	char *sofile = "/usr/lib64/ceph/compressor/libceph_snappy.so";
	if ((handle = dlopen(sofile, RTLD_NOW)) == NULL) {
		printf("%s: dlopen %s failed %s\n", __func__, sofile, dlerror());
		exit(-1);
	}
	printf("%s dlopen %s success\n", __func__, sofile);
	dlclose(handle);
	return 0;
}
