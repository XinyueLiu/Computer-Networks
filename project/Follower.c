#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "3643"  // the port users will be connecting to
#define TWEET_COUNT 3
#define FOLLOWER_COUNT 5
#define MAXDATASIZE 300 // max number of bytes we can get at once 
#define BACKLOG 10	 // how many pending connections queue will hold
#define SERVER_NAME "nunki.usc.edu"

#define FOLLOWER1_PHASE2_PORT "21743"	// UDP port used for Follower1 in phase 2
#define FOLLOWER2_PHASE2_PORT "21843"	// UDP port used for Follower2 in phase 2
#define FOLLOWER3_PHASE2_PORT "21943"	// UDP port used for Follower3 in phase 2
#define FOLLOWER4_PHASE2_PORT "22043"	// UDP port used for Follower4 in phase 2
#define FOLLOWER5_PHASE2_PORT "22143"	// UDP port used for Follower5 in phase 2

void getFollowerUDPport(int follower_id, char* follower_udpport){
	switch(follower_id){
		case 0: strcpy(follower_udpport, FOLLOWER1_PHASE2_PORT); break;
		case 1: strcpy(follower_udpport, FOLLOWER2_PHASE2_PORT); break;
		case 2: strcpy(follower_udpport, FOLLOWER3_PHASE2_PORT); break;
		case 3: strcpy(follower_udpport, FOLLOWER4_PHASE2_PORT); break;
		case 4: strcpy(follower_udpport, FOLLOWER5_PHASE2_PORT); break;
	}
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
}
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void getName(char *message, char *name) {
	char *ptr = strchr(message, ' ');
	int length = ptr - message;
	strncpy(name, message, length);
	name[length] = '\0';
	//printf("%s\n", name);
}

void getContent(char *message, char *content) {
	int totalLength=0;
	char* tmp = message;
    while(*tmp!='\0')
    {
        totalLength++;
        tmp++;
    }
	char *ptr = strchr(message, ' ');
	int len = ptr - message;
	int length = totalLength - len - 3;
	strncpy(content, ptr+1, length);
	content[length] = '\0';
	//printf("%s\n", content);
}

int main(void)
{
	int i; 
	struct hostent *host;
	struct in_addr **addr_list;
	char *host_addr;
	char feedback_name[5][14] = {"Follower1.txt","Follower2.txt","Follower3.txt","Follower4.txt","Follower5.txt"};

	// get host addr
	host = gethostbyname("nunki.usc.edu");
    addr_list = (struct in_addr **)host->h_addr_list;
    for(i = 0; addr_list[i] != NULL; i++) {
    	host_addr = inet_ntoa(*addr_list[i]);
    }

/**************************************************Phase II Start*****************************************************/
	/************************************1. Reveive Tweet from Server By UDP**************************************/
	i = 0;
	while(i < FOLLOWER_COUNT){ // create 5 follower processes
		pid_t wpid;
		int status = 0;
		if(!fork()){ // parent process wait for child process
			char follower_id[10];
			sprintf(follower_id, "Follower%d", i+1); // convert int to string
			char follower_udpport[10];
			getFollowerUDPport(i, follower_udpport);

	/****************************Beej's CODE START*********************************/
			struct addrinfo hints, *servinfo, *p;
			int sockfd;
			int rv, count;
			int numbytes;
			struct sockaddr_storage their_addr;
    		char buf[MAXDATASIZE];
    		socklen_t addr_len;

			memset(&hints, 0, sizeof hints);
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_DGRAM; //UDP
			hints.ai_flags = AI_PASSIVE; // use my IP

			if((rv = getaddrinfo(NULL, follower_udpport, &hints, &servinfo)) != 0) {
				fprintf(stderr, "Error: phase 2 getaddrinfo %s\n", gai_strerror(rv));
		        return 1;
			}

			// loop through all the results and find the first we can use
			for(p = servinfo; p != NULL; p = p->ai_next) {
				if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
		            perror("Error: phase 2 follower socket");
		            continue;
		        }

		        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            		close(sockfd);
            		perror("listener: bind");
					continue; 
				}
				break;
			}

			if (p == NULL) {
				fprintf(stderr, "Error: phase 2 follower failed to create socket\n");
				return 2;
			}
			freeaddrinfo(servinfo); // all done with this structure
			
			// wait to receive message from server using udp
			
			addr_len = sizeof their_addr;
			count = 0;
			while(count < TWEET_COUNT){
				char tweet_content[MAXDATASIZE];
				if ((numbytes = recvfrom(sockfd, buf, MAXDATASIZE-1 , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        			perror("recvfrom");
        			exit(1);
				}
				if(count == 0){
					printf("%s has UDP port number %s and IP address %s.\n", follower_id, follower_udpport, host_addr);
				}
				getContent(buf,tweet_content);
    			printf("%s has received tweet <%s>\n", follower_id, tweet_content);
    			count++;
			}
			close(sockfd);
			exit(0);
	/****************************Beej's CODE END*********************************/
		}
		while ((wpid = wait(&status)) > 0);	// wait for child process
		i++;
	}
	/******************************************Receive Tweet from Server By UDP Finish***************************************/

	/**********************************************2. Send feedback to Server By TCP*****************************************/
	i = 0;
	while(i < FOLLOWER_COUNT){ //create 5 followers
		pid_t wpid;
		int status = 0;
		if(!fork()){ //this is the child process
			char follower_id[10];	// tweet id
			int sockfd;  
			char buffer[MAXDATASIZE];
			struct addrinfo hints, *servinfo, *p;
			int rv;
			sprintf(follower_id, "Follower%d", i+1); // convert int to string

			FILE *fp;
			memset(&hints, 0, sizeof hints);
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_flags = AI_PASSIVE; // use my IP

	/****************************Beej's CODE START*********************************/
			if((rv = getaddrinfo(SERVER_NAME, PORT, &hints, &servinfo)) != 0) {
				fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
				return 1;
			}

			// loop through all the results and connect to the first we can
			for(p = servinfo; p != NULL; p = p->ai_next) {
				if ((sockfd = socket(p->ai_family, p->ai_socktype,
					p->ai_protocol)) == -1) {
					perror("Follower: socket");
					continue;
				}

				if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
					perror("Follower: connect");
					close(sockfd);
					continue;
				}

				break;
			}

	
			if (p == NULL) {
				fprintf(stderr, "Follower: failed to connect\n");
				return 2;
			}

	/****************************Beej's CODE END*********************************/
			// load Follower#.txt

			fp = fopen(feedback_name[i], "rt");	// open file for reading text
			while(fgets(buffer, MAXDATASIZE, fp) != NULL) {
				char message[MAXDATASIZE] = "";
				strcat(message, follower_id);
				strcat(message, " ");
				strcat(message, buffer);
				if (send(sockfd, message, strlen(message), 0) == -1){
					perror("send");
				}
				//printf("DEBUG: <%s> has sent <%s> to the server\n",follower_id, message);	
			}
			printf("Completed sending feedback for <%s>\n", follower_id);
			fclose(fp);
			freeaddrinfo(servinfo); // all done with this structure

			printf("End of Phase 2 for <%s>\n", follower_id);
			close(sockfd);
			exit(0);

		}
		while ((wpid = wait(&status)) > 0);	// wait for child process
		i++; 
	}
	/***************************************Send feedback to Server By TCP Finish*******************************************/


	return 0;
}
