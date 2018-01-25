CC = g++
LIBS = -lpthread
FLAGS = -g -Wall -std=c++11

.PHONY: all

all: client server

client: client.c
	$(CC) $(FLAGS) client.c -o client $(LIBS)

server: server.c
	$(CC) $(FLAGS) server.c -o server $(LIBS)



clean:
	rm -rf *.o server client
