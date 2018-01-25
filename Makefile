CC = g++
LIBS = -lpthread
FLAGS = -g -Wall -std=c++11

.PHONY: all

all: client server

client: client.c
	$(CC) $(FLAGS) $(LIB) client.c -o client

server: server.c
	$(CC) $(FLAGS) $(LIB) server.c -o server



clean:
	rm -rf *.o server client

