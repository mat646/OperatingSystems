CC=gcc -Wall

build-all:
	make build-task1
	make build-task2

build-task1:
	$(CC) ./Task1/client.c ./Task1/contract.h -o ./Task1/client
	$(CC) ./Task1/server.c ./Task1/contract.h -o ./Task1/server

build-task2:
	$(CC) ./Task2/client.c -o ./Task2/client
	$(CC) ./Task2/server.c -o ./Task2/server

run-task1:
	./Task1/server &
	./Task1/client

run-task2:
	./Task2/server &
	./Task2/client
