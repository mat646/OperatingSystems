.PHONY: all clean run

CC=gcc -Wall

all: client server

client: client.c contract.h
	$(CC) $^ -o $@ -lpthread -lm

server: server.c contract.h
	$(CC) $^ -o $@ -lpthread -lm

clean:
	rm client server socket1

run-server:
	./server 18080 socket1

run-client-1:
	./client c1 18080 127.0.0.1 1

run-client-2:
	./client c2 18080 socket1 0
