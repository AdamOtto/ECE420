all:
	g++ -g -Wall -o server server.c -lpthread
	g++ -g -Wall -o client client.c -lpthread 

clean:
	rm -rf *o server
	rm -rf *o client
