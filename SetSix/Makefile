CC=gcc -Wall

build-all:
	ipcrm --all=msg
	make build-task1
	make build-task2

build-task1:
	$(CC) ./Task1/client.c ./Task1/contract.h -o ./Task1/client
	$(CC) ./Task1/server.c ./Task1/contract.h -o ./Task1/server

build-task2:
	$(CC) ./Task2/client.c ./Task2/contract.h -o ./Task2/client -lrt
	$(CC) ./Task2/server.c ./Task2/contract.h -o ./Task2/server -lrt

run-task1:
	./Task1/server &
	./Task1/client

run-task2:
	./Task2/server &
	./Task2/client
