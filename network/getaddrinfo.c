
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUF_SIZE 500

int
main(int argc, char *argv[])
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    char buf[BUF_SIZE];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s ip\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET; // AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = 0; //SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = AI_ADDRCONFIG;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    const char *service = "nfs";

    printf(" ip %s\n", argv[1]);

    //s = getaddrinfo(argv[1], NULL, &hints, &result);
    s = getaddrinfo(argv[1], NULL, &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    char ipstr[16];
    int n = 0;
    for (rp = result; rp != NULL; rp = rp->ai_next) {
/*
        sfd = socket(rp->ai_family, rp->ai_socktype,
                rp->ai_protocol);
        if (sfd == -1)
            continue;
*/

	struct in_addr *inaddr = &(((struct sockaddr_in *)(rp->ai_addr))->sin_addr);
	inet_ntop(AF_INET, &(((struct sockaddr_in *)(rp->ai_addr))->sin_addr), ipstr, 16);
	uint32_t ip = inaddr->s_addr;
	//printf(" ip addr %lu\n", ip);
	printf(" ip addr dot %s\n", ipstr);
	n++;

        //if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
        //    break;                  /* Success */

 //       close(sfd);
    }
    printf("there are %d ip addresses\n", n);
    freeaddrinfo(result);
	return 0;
}
