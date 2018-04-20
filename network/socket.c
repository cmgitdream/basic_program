
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

int cmp_sockaddr(struct sockaddr_in *s1, struct sockaddr_in *s2)
{
 	return 0;
}

int main(int argc, char *argv[])
{
	struct sockaddr addr1;
	struct sockaddr_in addr2;
	struct sockaddr_in *adptr;

	memset(&addr1, 0, sizeof(struct sockaddr_in));
	memset(&addr2, 0, sizeof(struct sockaddr_in));
	
	adptr = (struct sockaddr_in *)&addr1;
	adptr->sin_family = AF_INET;
	adptr->sin_addr.s_addr = htonl(INADDR_ANY);
	adptr->sin_port = htons(2049);

	addr2.sin_family = AF_INET; 
	addr2.sin_addr.s_addr = htonl(INADDR_ANY);
	addr2.sin_port = htons(2049);

	//if (!memcmp((void *)adptr, (void *)&addr2, sizeof(struct sockaddr_in))) {
	if (!memcmp((void *)adptr, (void *)adptr, sizeof(struct sockaddr_in))) {
		printf("addr1 == addr2\n");
	} else {
		printf("addr1 != addr2\n");
	}
	return 0;
	
}
