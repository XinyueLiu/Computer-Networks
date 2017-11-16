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
#include <regex.h> 
#include <stdbool.h>

#define PORT "3643"  // the port users will be connecting to
#define TWEET_COUNT 3
#define FOLLOWER_COUNT 5
#define FEEDBEACK_COUNT 4
#define MAXDATASIZE 300 // max number of bytes we can get at once 
#define BACKLOG 10	 // how many pending connections queue will hold
#define SERVER_NAME "nunki.usc.edu"
#define LOCAL_TWEET_FILE "Tweet.txt"
#define LOCAL_FEEDBACK_FILE "Feedback.txt"
#define TWEETA_FEEDBACK "TweetA_F.txt"
#define TWEETB_FEEDBACK "TweetB_F.txt"
#define TWEETC_FEEDBACK "TweetC_F.txt"

#define FOLLOWER1_PHASE2_PORT "21743"	// UDP port used for Follower1 in phase 2
#define FOLLOWER2_PHASE2_PORT "21843"	// UDP port used for Follower2 in phase 2
#define FOLLOWER3_PHASE2_PORT "21943"	// UDP port used for Follower3 in phase 2
#define FOLLOWER4_PHASE2_PORT "22043"	// UDP port used for Follower4 in phase 2
#define FOLLOWER5_PHASE2_PORT "22143"	// UDP port used for Follower5 in phase 2

void getName(char *message, char *name) {
	char *ptr = strchr(message, ' ');
	int length = ptr - message;
	strncpy(name, message, length);
	name[length] = '\0';
	//printf("%s\n", name);
}

void extractFeedback(){
	FILE *fp;
	char buffer[MAXDATASIZE];
	char follower_name[10];
	// load Feedback.txt
	fp = fopen(LOCAL_FEEDBACK_FILE, "rt");	// open file for reading text
	while(fgets(buffer, MAXDATASIZE, fp) != NULL) {
		//printf("DEBUG: extractFeedback 1st string %s", buffer);
		bool followA = false;
		bool followB = false;
		bool followC = false;
		regex_t regex;
		int reti,i;
		char msgbuf[100];
		getName(buffer,follower_name);

		/*1. check who followed*/
		reti = regcomp(&regex, "Following:", 0);
		if (reti) {
    		fprintf(stderr, "Could not compile regex\n");
    		exit(1);
		}
		/* Execute regular expression */
		reti = regexec(&regex, buffer, 0, NULL, 0);
		if (!reti) {
			regex_t regex2;
			int reti2;
			reti2 = regcomp(&regex2, "TweetA", 0);
			if (reti2) {
    			fprintf(stderr, "Could not compile regex\n");
    			exit(1);
			}
			/* Execute regular expression */
			reti2 = regexec(&regex2, buffer, 0, NULL, 0);
			if (!reti2) {
				followA = true;
			}
			else if (reti2 != REG_NOMATCH) {
    			regerror(reti2, &regex2, msgbuf, sizeof(msgbuf));
    			fprintf(stderr, "Regex match failed: %s\n", msgbuf);
    			exit(1);
			}

			reti2 = regcomp(&regex2, "TweetB", 0);
			if (reti2) {
    			fprintf(stderr, "Could not compile regex\n");
    			exit(1);
			}
			/* Execute regular expression */
			reti2 = regexec(&regex2, buffer, 0, NULL, 0);
			if (!reti2) {
				followB = true;
			}
			else if (reti2 != REG_NOMATCH) {
    			regerror(reti2, &regex2, msgbuf, sizeof(msgbuf));
    			fprintf(stderr, "Regex match failed: %s\n", msgbuf);
    			exit(1);
			}

			reti2 = regcomp(&regex2, "TweetC", 0);
			if (reti2) {
    			fprintf(stderr, "Could not compile regex\n");
    			exit(1);
			}
			/* Execute regular expression */
			reti2 = regexec(&regex2, buffer, 0, NULL, 0);
			if (!reti2) {
				followC = true;
			}
			else if (reti2 != REG_NOMATCH) {
    			regerror(reti2, &regex2, msgbuf, sizeof(msgbuf));
    			fprintf(stderr, "Regex match failed: %s\n", msgbuf);
    			exit(1);
			}

		}
		else if (reti != REG_NOMATCH) {
    		regerror(reti, &regex, msgbuf, sizeof(msgbuf));
    		fprintf(stderr, "Regex match failed: %s\n", msgbuf);
    		exit(1);
		}

		/*2. check who liked*/
		i = 0;
		while(i < FEEDBEACK_COUNT-1 && fgets(buffer, MAXDATASIZE, fp) != NULL) {
			
			/* Compile regular expression */
			//printf("DEBUG: extractFeedback 2-4 strigs: %s", buffer);
			reti = regcomp(&regex, "TweetA:like", 0);
			if (reti) {
    			fprintf(stderr, "Could not compile regex\n");
    			exit(1);
			}
			/* Execute regular expression */
			reti = regexec(&regex, buffer, 0, NULL, 0);
			if (!reti) {
				FILE *fp;
    			fp = fopen(TWEETA_FEEDBACK, "a");
    			fputs(follower_name,fp);
    			fputs("#like",fp);
    			fputs("\n", fp);
				fclose(fp);
				followA = false;
			}
			else if (reti != REG_NOMATCH) {
    			regerror(reti, &regex, msgbuf, sizeof(msgbuf));
    			fprintf(stderr, "Regex match failed: %s\n", msgbuf);
    			exit(1);
			}

			reti = regcomp(&regex, "TweetB:like", 0);
			if (reti) {
    			fprintf(stderr, "Could not compile regex\n");
    			exit(1);
			}
			/* Execute regular expression */
			reti = regexec(&regex, buffer, 0, NULL, 0);
			if (!reti) {
				FILE *fp;
    			fp = fopen(TWEETB_FEEDBACK, "a");
    			fputs(follower_name,fp);
    			fputs("#like",fp);
    			fputs("\n", fp);
				fclose(fp);
				followB = false;
			}
			else if (reti != REG_NOMATCH) {
    			regerror(reti, &regex, msgbuf, sizeof(msgbuf));
    			fprintf(stderr, "Regex match failed: %s\n", msgbuf);
    			exit(1);
			}

			reti = regcomp(&regex, "TweetC:like", 0);
			if (reti) {
    			fprintf(stderr, "Could not compile regex\n");
    			exit(1);
			}
			/* Execute regular expression */
			reti = regexec(&regex, buffer, 0, NULL, 0);
			if (!reti) {
				FILE *fp;
    			fp = fopen(TWEETC_FEEDBACK, "a");
    			fputs(follower_name,fp);
    			fputs("#like",fp);
    			fputs("\n", fp);
				fclose(fp);
				followC = false;
			}
			else if (reti != REG_NOMATCH) {
    			regerror(reti, &regex, msgbuf, sizeof(msgbuf));
    			fprintf(stderr, "Regex match failed: %s\n", msgbuf);
    			exit(1);
			}
			i++;
		}
		
		if(followA == true){
			FILE *fp;
    		fp = fopen(TWEETA_FEEDBACK, "a");
    		fputs(follower_name,fp);
    		fputs("\n", fp);
			fclose(fp);
		}

		if(followB == true){
			FILE *fp;
    		fp = fopen(TWEETB_FEEDBACK, "a");
    		fputs(follower_name,fp);
    		fputs("\n", fp);
			fclose(fp);
		}

		if(followC == true){
			FILE *fp;
    		fp = fopen(TWEETC_FEEDBACK, "a");
    		fputs(follower_name,fp);
    		fputs("\n", fp);
			fclose(fp);
		}
		
		/* Free memory allocated to the pattern buffer by regcomp() */
		regfree(&regex);

	}
	fclose(fp);

}

void getFollowerUDPport(int id, char* follower_udpport){
	switch(id){
		case 0: strcpy(follower_udpport, FOLLOWER1_PHASE2_PORT); break;
		case 1: strcpy(follower_udpport, FOLLOWER2_PHASE2_PORT); break;
		case 2: strcpy(follower_udpport, FOLLOWER3_PHASE2_PORT); break;
		case 3: strcpy(follower_udpport, FOLLOWER4_PHASE2_PORT); break;
		case 4: strcpy(follower_udpport, FOLLOWER5_PHASE2_PORT); break;
	}
}

void sigchld_handler(int s)
{
	// waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;

	while(waitpid(-1, NULL, WNOHANG) > 0);

	errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
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

void printLocalTweet(){
	FILE *fp;
	char buffer[MAXDATASIZE];
	char tweet_name[10];
	char tweet_content[MAXDATASIZE];
	// load Tweet.txt
	fp = fopen(LOCAL_TWEET_FILE, "rt");	// open file for reading text
	while(fgets(buffer, MAXDATASIZE, fp) != NULL) {
		// // remove '\n\r'
		int length = strlen(buffer)-1;
		if (buffer[length] == '\n') buffer[length] = '\0';
		if (buffer[length] == '\r') buffer[length] = '\0';
		// printf("%s\n", buffer);
		getName(buffer,tweet_name);
		getContent(buffer,tweet_content);
		printf("Received the tweet <%s> from <%s>\n",tweet_content, tweet_name);
	}
	fclose(fp);
}

int main(void)
{

	int sockfd, new_fd, i;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	struct hostent *host;
	struct in_addr **addr_list;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;
	char buf[MAXDATASIZE];
	char *host_addr;
	char tweet_id[3][7] = {"TweetA","TweetB","TweetC"};
	char fbfile_name[3][13] = {"TweetA_F.txt","TweetB_F.txt","TweetC_F.txt"};

/**************************************************Phase I Start*****************************************************/
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM; //TCP
	hints.ai_flags = AI_PASSIVE; // use my IP

/****************************Beej's CODE START*********************************/
	// get host addr
	host = gethostbyname("nunki.usc.edu");
    addr_list = (struct in_addr **)host->h_addr_list;
    for(i = 0; addr_list[i] != NULL; i++) {
    	host_addr = inet_ntoa(*addr_list[i]);
    }

    
    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
			p->ai_protocol)) == -1) {
			perror("Error: phase 1 server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
			sizeof(int)) == -1) {
			perror("Error: phase 1 setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("Error: phase 1 server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL)  {
		fprintf(stderr, "Error: phase 1 server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("Error: phase 1 listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("Error: phase 1 sigaction");
		exit(1);
	}

/****************************Beej's CODE END*********************************/

	printf("The server has TCP port %s and IP address %s.\n", PORT, host_addr);

	int count = 0;
	//printf("server: waiting for connections...\n");
	// wait for connections
	while(1) {  // main accept() loop
		pid_t wpid;
		int status = 0;
		if(count == TWEET_COUNT) break;
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}

		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s);

		if (!fork()) { // this is the child process
			close(sockfd); // child doesn't need the listener
			if (recv(new_fd, buf, MAXDATASIZE-1, 0) != -1) {
				char tweet_name[10];
				char tweet_content[MAXDATASIZE];
				FILE *fp;

				int length = strlen(buf)-1;
				if (buf[length] == '\n') buf[length] = '\0';
				if (buf[length] == '\r') buf[length] = '\0';

				getName(buf, tweet_name);
				getContent(buf, tweet_content);

				fp = fopen(LOCAL_TWEET_FILE, "a");
				fputs(tweet_name, fp);
				fputs(" ", fp);
				fputs(tweet_content, fp);
				fputs("\n", fp);
				fclose(fp);
				//printf("DEBUG: Received the tweet <%s> from <%s>\n",tweet_content, tweet_name);
			}else{
				perror("Error: recv");
				exit(1);
			}
			close(new_fd);
			exit(0);
			
		}
		while ((wpid = wait(&status)) > 0); // wait for child process
		close(new_fd);  // parent doesn't need this
		count++;
	}
	
	close(sockfd);
	printLocalTweet();
	printf("End of Phase 1 for the server\n");

/**************************************************Phase II Start*****************************************************/
	/************************************1. Send Tweet to Followers By UDP*****************************************/
	i = 0;
	while(i < FOLLOWER_COUNT){ // create 5 server processes
		pid_t wpid;
		int status = 0;
		if(!fork()){ // parent process wait for child process
			char follower_id[10];
			sprintf(follower_id, "Follower%d", i+1); // convert int to string
			char follower_udpport[10];
			getFollowerUDPport(i, follower_udpport);

			FILE *fp;
			char buffer[MAXDATASIZE];

	/****************************Beej's CODE START*********************************/
			struct addrinfo hints1, *servinfo1, *p1;
			int sfd;
			memset(&hints1, 0, sizeof hints1);
			hints1.ai_family = AF_UNSPEC;
			hints1.ai_socktype = SOCK_DGRAM; //UDP
			int numbytes;

			if((rv = getaddrinfo(SERVER_NAME, follower_udpport, &hints1, &servinfo1)) != 0) {
				fprintf(stderr, "Error: phase 2 getaddrinfo %s\n", gai_strerror(rv));
		        return 1;
			}

			// loop through all the results and bind to the first we can
			for(p1 = servinfo1; p1 != NULL; p1 = p1->ai_next) {
				if ((sfd = socket(p1->ai_family, p1->ai_socktype, p1->ai_protocol)) == -1) {
		            perror("Error: phase 2 server: socket");
		            continue;
		        }
				break;
			}

			if (p1 == NULL) {
				fprintf(stderr, "Error: phase 2 server failed to bind socket\n");
				return 2;
			}

			// send message to follower using udp
			fp = fopen(LOCAL_TWEET_FILE, "rt");	// open file for reading text
			while(fgets(buffer, MAXDATASIZE, fp) != NULL) {
				// // remove '\n\r'
				char tweet_content[MAXDATASIZE];
				int length = strlen(buffer)-1;
				if (buffer[length] == '\n') buffer[length] = '\0';
				if (buffer[length] == '\r') buffer[length] = '\0';
				// printf("%s\n", buffer);
				if((numbytes = sendto(sfd, buffer, strlen(buffer), 0, p1->ai_addr, p1->ai_addrlen)) == -1) {
					perror("Error: phase 2 server: sendto");
	    	  		exit(1);
				}
				// get dynamic address
				char address[INET_ADDRSTRLEN];
				inet_ntop(p1->ai_family, get_in_addr((struct sockaddr *)p1->ai_addr),address, sizeof address);
				// get dynamic port
				int port;
				struct sockaddr_in sin;
				socklen_t len = sizeof(sin);
				if (getsockname(sfd, (struct sockaddr *)&sin, &len) == -1){
					perror("Error: getsockname");
				}else{
					port = ntohs(sin.sin_port);
				}
				printf("The server has UDP port number %d and IP address %s.\n", port, address);

				getContent(buffer,tweet_content);
				printf("The server has sent <%s> to the %s.\n", tweet_content, follower_id);
			}
			fclose(fp);

			freeaddrinfo(servinfo1); // all done with this structure
			close(sfd);
			exit(0);
	/****************************Beej's CODE END*********************************/
		}
		while ((wpid = wait(&status)) > 0);	// wait for child process
		i++;
	}
	/*******************************************Send Tweet to Followers By UDP Finish***************************************/

	/******************************************2. Receive feedback from Follower By TCP*************************************/
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM; //TCP
	hints.ai_flags = AI_PASSIVE; // use my IP

	/****************************Beej's CODE START*********************************/
	// get host addr
	host = gethostbyname("nunki.usc.edu");
    addr_list = (struct in_addr **)host->h_addr_list;
    for(i = 0; addr_list[i] != NULL; i++) {
    	host_addr = inet_ntoa(*addr_list[i]);
    }

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
			p->ai_protocol)) == -1) {
			perror("Error: phase 1 server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
			sizeof(int)) == -1) {
			perror("Error: phase 1 setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("Error: phase 1 server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL)  {
		fprintf(stderr, "Error: phase 1 server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("Error: phase 1 listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("Error: phase 1 sigaction");
		exit(1);
	}

	/****************************Beej's CODE END*********************************/

	printf("The server has TCP port %s and IP address %s.\n", PORT, host_addr);

	count = 0;
	//printf("server: waiting for connections...\n");
	// wait for connections
	while(1) {  // main accept() loop
		pid_t wpid;
		int status = 0;
		if(count == FOLLOWER_COUNT) break;
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}

		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s);

		if (!fork()) { // this is the child process
			close(sockfd); // child doesn't need the listener
			if(recv(new_fd, buf, MAXDATASIZE-1, 0) != -1) {
				char follower_name[10];
				FILE *fp;
				getName(buf, follower_name);
				fp = fopen(LOCAL_FEEDBACK_FILE, "a");
				fputs(buf,fp);
				fputs("\n", fp);
				fclose(fp);
				printf("Server receive the feedback from %s\n", follower_name);
				
			}else{
				perror("Error: recv");
				exit(1);
			}
			close(new_fd);
			exit(0);
			
		}
		while ((wpid = wait(&status)) > 0); // wait for child process
		close(new_fd);  // parent doesn't need this
		count++;
	}
	close(sockfd);
	/*******************************Receive feedback from Follower By TCP Finsih******************************************/

	/******************************************3. Send Feedback to Tweet by TCP*************************************/
	extractFeedback();

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM; //TCP
	hints.ai_flags = AI_PASSIVE; // use my IP

	/****************************Beej's CODE START*********************************/
	// get host addr
	host = gethostbyname("nunki.usc.edu");
    addr_list = (struct in_addr **)host->h_addr_list;
    for(i = 0; addr_list[i] != NULL; i++) {
    	host_addr = inet_ntoa(*addr_list[i]);
    }

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
			p->ai_protocol)) == -1) {
			perror("Error: phase 2 server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
			sizeof(int)) == -1) {
			perror("Error: phase 2 setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("Error: phase 2 server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL)  {
		fprintf(stderr, "Error: phase 2 server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("Error: phase 2 listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("Error: phase 2 sigaction");
		exit(1);
	}

	/****************************Beej's CODE END*********************************/

	printf("The server has TCP port %s and IP address %s.\n", PORT, host_addr);

	count = 0;
	//printf("server: waiting for connections...\n");
	// wait for connections
	while(count < TWEET_COUNT){  // main accept() loop
		pid_t wpid;
		int status = 0;		sin_size = sizeof their_addr;
		
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}
		//printf("DEBUG: Accept Someone.\n");
		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s);

		if (!fork()) { // this is the child process
			close(sockfd); // child doesn't need the listener
			// load Tweet#_F.txt
			FILE *fp;
			char buffer[MAXDATASIZE];
			fp = fopen(fbfile_name[count], "rt");	// open file for reading text
			while(fgets(buffer, MAXDATASIZE, fp) != NULL) {
				if (send(new_fd, buffer, strlen(buffer), 0) == -1){
					perror("send");
				}
				//printf("DEBUG: The server has send the feedback <%s>\n", buffer);
			}
			if (send(new_fd, "END\n", strlen("END"), 0) == -1){
					perror("send");
			}
			printf("The server has send the feedback result to <%s>\n", tweet_id[count]);
			fclose(fp);
			close(new_fd);
			exit(0);
			
		}
		while ((wpid = wait(&status)) > 0); // wait for child process
		close(new_fd);  // parent doesn't need this
		count++;
	}
	close(sockfd);
	printf("End of Phase 2 for server\n");
    /*****************************************Send Feedback to Tweet by TCP Finish**********************************/

		
	return 0;
}
