a. Name: XINYUE LIU
b. USCID: 1332044343

c. What have I done in this assignment:
	Implement Phase 2 successfully.
	Using fork() to create 3 Tweets and 5 followers.

d. What each file does:
	There are three files in total:
	1. Server.c   -> (Phase 1)
					 implement as a server side 
				     Server will wait for a connection, accept() it, and fork() a child process to receive the tweet message,
					 and store it in local file (Tweet.txt). 
					 (Phase 2)
					 Send all tweets to each follower throught UDP
					 Receive all feedback from followers throught TCP
					 Classify and send feedbacks to corresponding Tweet through TCP

	2. Tweet.c    -> (Phase1)
					 implement as a client side 
					 Tweet will create three different connections with Server by using fork, each of them will send the corresponding tweet message (including current tweet_id (i.g. TweetA)). 
					 (Phase2)
					 Receive feedbacks from Server store in local file (T#_FB.txt)

	3. Follower.c -> (Phase2)
					  Receive all tweets from server through UDP
					  Send feedback to Server throught TCP

e. How to run the programs:
    1. First you should open three different terminal. And using one of them to type "make all" to compile all the file. 
	2. typing "Server" using one terminal to run Server.c.
	3. typing "Follower" using one terminal to run Follower.c.
	3. typing "Tweet" using one terminal to run Tweet.c.
   (4. typing "make clean" to clean all the *.o file and *.txt) 

f. The format of all the messages:

1. Server:
The server has TCP port 3643 and IP address 68.181.201.3.
Received the tweet <A: The distance from UCLA to best University is less than 15 mi> from <TweetA>
Received the tweet <B: The distance from UCLA to best University is less than 15 mi> from <TweetB>
Received the tweet <C: The distance from UCLA to best University is less than 15 mi> from <TweetC>
End of Phase 1 for the server
The server has UDP port number 37531 and IP address 68.181.201.3.
The server has sent <A: The distance from UCLA to best University is less than 15 mi> to the Follower1.
The server has UDP port number 37531 and IP address 68.181.201.3.
The server has sent <B: The distance from UCLA to best University is less than 15 mi> to the Follower1.
The server has UDP port number 37531 and IP address 68.181.201.3.
The server has sent <C: The distance from UCLA to best University is less than 15 mi> to the Follower1.
The server has UDP port number 37532 and IP address 68.181.201.3.
The server has sent <A: The distance from UCLA to best University is less than 15 mi> to the Follower2.
The server has UDP port number 37532 and IP address 68.181.201.3.
The server has sent <B: The distance from UCLA to best University is less than 15 mi> to the Follower2.
The server has UDP port number 37532 and IP address 68.181.201.3.
The server has sent <C: The distance from UCLA to best University is less than 15 mi> to the Follower2.
The server has UDP port number 37533 and IP address 68.181.201.3.
The server has sent <A: The distance from UCLA to best University is less than 15 mi> to the Follower3.
The server has UDP port number 37533 and IP address 68.181.201.3.
The server has sent <B: The distance from UCLA to best University is less than 15 mi> to the Follower3.
The server has UDP port number 37533 and IP address 68.181.201.3.
The server has sent <C: The distance from UCLA to best University is less than 15 mi> to the Follower3.
The server has UDP port number 37534 and IP address 68.181.201.3.
The server has sent <A: The distance from UCLA to best University is less than 15 mi> to the Follower4.
The server has UDP port number 37534 and IP address 68.181.201.3.
The server has sent <B: The distance from UCLA to best University is less than 15 mi> to the Follower4.
The server has UDP port number 37534 and IP address 68.181.201.3.
The server has sent <C: The distance from UCLA to best University is less than 15 mi> to the Follower4.
The server has UDP port number 37535 and IP address 68.181.201.3.
The server has sent <A: The distance from UCLA to best University is less than 15 mi> to the Follower5.
The server has UDP port number 37535 and IP address 68.181.201.3.
The server has sent <B: The distance from UCLA to best University is less than 15 mi> to the Follower5.
The server has UDP port number 37535 and IP address 68.181.201.3.
The server has sent <C: The distance from UCLA to best University is less than 15 mi> to the Follower5.
The server has TCP port 3643 and IP address 68.181.201.3.
Server receive the feedback from Follower1
Server receive the feedback from Follower2
Server receive the feedback from Follower3
Server receive the feedback from Follower4
Server receive the feedback from Follower5
The server has TCP port 3643 and IP address 68.181.201.3.
The server has send the feedback result to <TweetA>
The server has send the feedback result to <TweetB>
The server has send the feedback result to <TweetC>
End of Phase 2 for server

Note: the above specific tweet messages will be changed based on different input Tweet#.txt files.
Note: the above specific feedback messages will be changed based on different input Follower#.txt files.

2. Tweet:
TweetA has TCP port 42041 and IP address 68.181.201.3 for Phase 1.
<TweetA> is now connected to server
<TweetA> has sent <A: The distance from UCLA to best University is less than 15 miles.> to the server
Updating the server is done for <TweetA>
End of Phase 1 for <TweetA>
TweetB has TCP port 42042 and IP address 68.181.201.3 for Phase 1.
<TweetB> is now connected to server
<TweetB> has sent <B: The distance from UCLA to best University is less than 15 miles.> to the server
Updating the server is done for <TweetB>
End of Phase 1 for <TweetB>
TweetC has TCP port 42043 and IP address 68.181.201.3 for Phase 1.
<TweetC> is now connected to server
<TweetC> has sent <C: The distance from UCLA to best University is less than 15 miles.> to the server
Updating the server is done for <TweetC>
End of Phase 1 for <TweetC>
TweetA has TCP port 42085 and IP address 68.181.201.3 for Phase 2.
<Follower1> has liked <TweetA>
<Follower2> is following <TweetA>
<Follower5> has liked <TweetA>
End of Phase 2 for <TweetA>
TweetB has TCP port 42091 and IP address 68.181.201.3 for Phase 2.
<Follower1> is following <TweetB>
<Follower2> has liked <TweetB>
<Follower3> has liked <TweetB>
<Follower4> is following <TweetB>
<Follower5> has liked <TweetB>
End of Phase 2 for <TweetB>
TweetC has TCP port 42092 and IP address 68.181.201.3 for Phase 2.
<Follower1> is following <TweetC>
<Follower3> has liked <TweetC>
<Follower5> has liked <TweetC>
End of Phase 2 for <TweetC>

Note: the above specific tweet messages will be changed based on different input Tweet#.txt files.


3. Follower:
Follower1 has UDP port number 21743 and IP address 68.181.201.3.
Follower1 has received tweet <A: The distance from UCLA to best University is less than 15 mi>
Follower1 has received tweet <B: The distance from UCLA to best University is less than 15 mi>
Follower1 has received tweet <C: The distance from UCLA to best University is less than 15 mi>
Follower2 has UDP port number 21843 and IP address 68.181.201.3.
Follower2 has received tweet <A: The distance from UCLA to best University is less than 15 mi>
Follower2 has received tweet <B: The distance from UCLA to best University is less than 15 mi>
Follower2 has received tweet <C: The distance from UCLA to best University is less than 15 mi>
Follower3 has UDP port number 21943 and IP address 68.181.201.3.
Follower3 has received tweet <A: The distance from UCLA to best University is less than 15 mi>
Follower3 has received tweet <B: The distance from UCLA to best University is less than 15 mi>
Follower3 has received tweet <C: The distance from UCLA to best University is less than 15 mi>
Follower4 has UDP port number 22043 and IP address 68.181.201.3.
Follower4 has received tweet <A: The distance from UCLA to best University is less than 15 mi>
Follower4 has received tweet <B: The distance from UCLA to best University is less than 15 mi>
Follower4 has received tweet <C: The distance from UCLA to best University is less than 15 mi>
Follower5 has UDP port number 22143 and IP address 68.181.201.3.
Follower5 has received tweet <A: The distance from UCLA to best University is less than 15 mi>
Follower5 has received tweet <B: The distance from UCLA to best University is less than 15 mi>
Follower5 has received tweet <C: The distance from UCLA to best University is less than 15 mi>
Completed sending feedback for <Follower1>
End of Phase 2 for <Follower1>
Completed sending feedback for <Follower2>
End of Phase 2 for <Follower2>
Completed sending feedback for <Follower3>
End of Phase 2 for <Follower3>
Completed sending feedback for <Follower4>
End of Phase 2 for <Follower4>
Completed sending feedback for <Follower5>
End of Phase 2 for <Follower5>

Note: the above specific tweet messages will be changed based on different input Tweet#.txt files.

g. The idiosyncrasy:
	Under my test case, I find no error.

h. Reuse of the code:
	Some code blocks for setting TCP are from "Beej's Guide to Network Programming -- Using Internet Sockets". They are marked in the program. 


