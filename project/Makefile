all:
	gcc -g -Wall -o Tweet Tweet.c -lsocket -lnsl -lresolv
	gcc -g -Wall -o Server Server.c -lsocket -lnsl -lresolv
	gcc -g -Wall -o Follower Follower.c -lsocket -lnsl -lresolv
clean:
	rm -f *.o Tweet.txt Feedback.txt TweetA_F.txt TweetB_F.txt TweetC_F.txt TB_FB.txt TA_FB.txt TC_FB.txt Tweet Server Follower
