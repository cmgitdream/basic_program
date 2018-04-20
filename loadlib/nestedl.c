
#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>

void usage()
{
	printf("./dl /path/to/so\n");
}

int main(int argc, char *argv[])
{
	void *handle = NULL;
	void (*callfunc)();
	char *errstr;
	char *sofile = "./testdl.so";
	if ((handle = dlopen(sofile, RTLD_NOW)) == NULL) {
		printf("dlopen %s failed %s\n", sofile, dlerror());
		exit(-1);
	}
	printf(" dlopen %s success\n", sofile);

	printf(" call func of %s\n", sofile);
	callfunc = dlsym(handle, "open_dl");	
	if ((errstr = dlerror() )!= NULL)
	{
		printf("%s\n", errstr);
		exit(1);
	}
	callfunc();
	dlclose(handle);
	
	return 0;
}
