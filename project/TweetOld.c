/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT "3643" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
	int sockfd, numbytes;  
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	if (argc != 2) {
	    fprintf(stderr,"usage: client hostname\n");
	    exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			perror("client: connect");
			close(sockfd);
			continue;
		}

		break;
	}

	
	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	// get dynamic address
	char address[INET_ADDRSTRLEN];
	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),address, sizeof address);
	// get dynamic port
	int port;
	struct sockaddr_in sin;
	socklen_t len = sizeof(sin);
	if (getsockname(sockfd, (struct sockaddr *)&sin, &len) == -1)
				perror("Error: phase 3 getsockname");
			else
    			port = ntohs(sin.sin_port);

    printf("Phase 1: Tweet# has dynamic TCP port number %d and IP address %s.\n", port, address);


	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof s);
	printf("<Tweet#> is now connected to server: %s\n", s);
	if (send(sockfd, "Hello, world too!", 18, 0) == -1){
		perror("send");
	}else{
		printf("<Tweet#> has sent <Hello, world too!> to the server\n");
	}
				

	freeaddrinfo(servinfo); // all done with this structure

	// if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	//     perror("recv");
	//     exit(1);
	// }else{
	// 	buf[numbytes] = '\0';
	// 	printf("client: received '%s'\n",buf);
	// }

	printf("End of Phase 1 for <Tweet#>\n");
	close(sockfd);
	
	return 0;
}
