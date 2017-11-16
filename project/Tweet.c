#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <regex.h> 

#define PORT "3643" // the port client will be connecting to 
#define TWEET_COUNT 3
#define MAXDATASIZE 300 // max number of bytes we can get at once 
#define SERVER_NAME "nunki.usc.edu"
#define FEEDBEACK_COUNT 4

// get sockaddr, IPv4 or IPv6:
void printFeedback(char* Feedback, char* tweet_id){
	FILE *fp;
	char buffer[MAXDATASIZE];
	char ff_name[10];
	char follower_name[10];
	strcpy(ff_name,Feedback);
	regex_t regex, regex2;
	int reti, reti2;

	reti = regcomp(&regex, "like", 0);
	if (reti) {
    	fprintf(stderr, "Could not compile regex\n");
    	exit(1);
	}

	reti2 = regcomp(&regex2, "END", 0);
	if (reti2) {
    	fprintf(stderr, "Could not compile regex\n");
    	exit(1);
	}

	fp = fopen(ff_name, "rt");	// open file for reading text
	while(fgets(buffer, MAXDATASIZE, fp) != NULL) {
		if(!regexec(&regex2, buffer, 0, NULL, 0)) break;
		reti = regexec(&regex, buffer, 0, NULL, 0);
		if (!reti) {
			char *ptr = strchr(buffer, '#');
			int length = ptr - buffer;
			strncpy(follower_name, buffer, length);
			follower_name[length] = '\0';
			char *ptr2 = strchr(follower_name, 'F');
			printf("<%s> has liked <%s>\n", ptr2, tweet_id);
		}else{
			int length = strlen(buffer)-1;
			if (buffer[length] == '\n') buffer[length] = '\0';
			if (buffer[length] == '\r') buffer[length] = '\0';
			char *ptr = strchr(buffer, 'F');
			printf("<%s> is following <%s>\n", ptr, tweet_id);
		}
	}
	fclose(fp);

}
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
	int i = 0;
	char tweet_name[3][7] = {"TweetA","TweetB","TweetC"};
	char file_name[3][11] = {"TweetA.txt","TweetB.txt","TweetC.txt"};
	char Tweet_Feedback[4][10] = {"A","TA_FB.txt","TB_FB.txt","TC_FB.txt"};

	while(i < TWEET_COUNT){ //create 3 Tweets
		pid_t wpid;
		int status = 0;
		if(!fork()){ //this is the child process
			char tweet_id[10];	// tweet id
			int sockfd;  
			char buffer[MAXDATASIZE];
			struct addrinfo hints, *servinfo, *p;
			int rv;
			strcpy(tweet_id,tweet_name[i]);
			FILE *fp;
/************************************************Phase I: Send Tweet To Server*************************************************/
			memset(&hints, 0, sizeof hints);
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_flags = AI_PASSIVE; // use my IP

/****************************Beej's CODE START*********************************/
			if ((rv = getaddrinfo(SERVER_NAME, PORT, &hints, &servinfo)) != 0) {
				fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
				return 1;
			}

			// loop through all the results and connect to the first we can
			for(p = servinfo; p != NULL; p = p->ai_next) {
				if ((sockfd = socket(p->ai_family, p->ai_socktype,
					p->ai_protocol)) == -1) {
					perror("Tweet: socket");
					continue;
				}

				if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
					perror("Tweet: connect");
					close(sockfd);
					continue;
				}

				break;
			}

	
			if (p == NULL) {
				fprintf(stderr, "Tweet: failed to connect\n");
				return 2;
			}

/****************************Beej's CODE END*********************************/

			// get dynamic address
			char address[INET_ADDRSTRLEN];
			inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),address, sizeof address);
			// get dynamic port
			int port;
			struct sockaddr_in sin;
			socklen_t len = sizeof(sin);
			if (getsockname(sockfd, (struct sockaddr *)&sin, &len) == -1){
				perror("Error: getsockname");
			}else{
				port = ntohs(sin.sin_port);
			}
    			
    		printf("%s has TCP port %d and IP address %s for Phase 1.\n", tweet_id, port, address);


			//inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),s, sizeof s);
			printf("<%s> is now connected to server\n", tweet_id);
			
			// load Tweet#.txt
			fp = fopen(file_name[i], "rt");	// open file for reading text
			if(fgets(buffer, MAXDATASIZE, fp) != NULL) {
				// // remove '\n\r'
				int length = strlen(buffer)-1;
				if (buffer[length] == '\n') buffer[length] = '\0';
				if (buffer[length] == '\r') buffer[length] = '\0';
				// printf("%s\n", buffer);
				char message[MAXDATASIZE] = "";
				strcat(message, tweet_id);
				strcat(message, " ");
				strcat(message, buffer);
				if (send(sockfd, message, strlen(message), 0) == -1){
					perror("send");
				}else{
					char *ptr = strchr(message, ' ');
					printf("<%s> has sent <%s> to the server\n",tweet_id, ++ptr);
					printf("Updating the server is done for <%s>\n", tweet_id);
				}
			
			}
			fclose(fp);
			freeaddrinfo(servinfo); // all done with this structure

			printf("End of Phase 1 for <%s>\n", tweet_id);
			close(sockfd);
			exit(0);

		}
		while ((wpid = wait(&status)) > 0);	// wait for child process
		i++;
	}
	/*****************************************Phase II receive Feedback from Server*****************************************/
	i = 0;
	while(i < TWEET_COUNT+1){ //create 3 Tweets + 1 test
		pid_t wpid;
		int status = 0;
		if(!fork()){ //this is the child process
			char tweet_id[10];	// tweet id
			int sockfd,count;  
			char buffer[MAXDATASIZE];
			struct addrinfo hints, *servinfo, *p;
			int rv;
			if(i > 0){
				strcpy(tweet_id,tweet_name[i-1]);
			}
			memset(&hints, 0, sizeof hints);
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_flags = AI_PASSIVE; // use my IP

/****************************Beej's CODE START*********************************/
			
			p = NULL;
			// loop through all the results and connect to the first we can
			while(p == NULL){
				//printf("DEBUG: Trying to connect\n");
				if ((rv = getaddrinfo(SERVER_NAME, PORT, &hints, &servinfo)) != 0) {
					fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
					return 1;
				}
				for(p = servinfo; p != NULL; p = p->ai_next) {
					if ((sockfd = socket(p->ai_family, p->ai_socktype,
						p->ai_protocol)) == -1) {
						//perror("Tweet: socket");
						continue;
					}

					if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
						//perror("Tweet: connect");
						close(sockfd);
						continue;
					}
					break;
				}
				if (p != NULL) {
					break;
				}
			}
	
			

/****************************Beej's CODE END*********************************/
			//printf("DEBUG: Successfully connect, %s\n", p);
			// get dynamic address
			char address[INET_ADDRSTRLEN];
			inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),address, sizeof address);
			// get dynamic port
			int port;
			struct sockaddr_in sin;
			socklen_t len = sizeof(sin);
			if (getsockname(sockfd, (struct sockaddr *)&sin, &len) == -1){
				perror("Error: getsockname");
			}else{
				port = ntohs(sin.sin_port);
			}
    		
    		if(i > 0){
    			printf("%s has TCP port %d and IP address %s for Phase 2.\n", tweet_id, port, address);
    		}
			count = 0;
			while(count < 2){
				if (recv(sockfd, buffer, MAXDATASIZE, 0) != -1) {
					if(i > 0){
						FILE *fp;
						fp = fopen(Tweet_Feedback[i], "a");
						fputs(buffer, fp);
						//printf("PUT BUFFER: %s\n in %s",buffer, Tweet_Feedback);
						fclose(fp);
					}	
				}
				count++;
				if(count == 2){
					printFeedback(Tweet_Feedback[i], tweet_id);
					printf("End of Phase 2 for <%s>\n", tweet_id);
				}
			}
			freeaddrinfo(servinfo); // all done with this structure
			close(sockfd);
			exit(0);

		}
		while ((wpid = wait(&status)) > 0);	// wait for child process
		i++;
	}

	return 0;
}
