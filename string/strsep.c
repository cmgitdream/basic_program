#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	char ip_str[] = "192.168.1.250";
	char *p = ip_str;
	char *ip_arr[4];
	char * s = strsep(&p, ".");
	int i=0;
	while(s)
	{
		ip_arr[i] = s;
		s = strsep(&p, ".");
		i++;
//		printf("%s\n",s);
	}

	for(i=0; i<4; i++)
		printf("%s\n",ip_arr[i]);
}
