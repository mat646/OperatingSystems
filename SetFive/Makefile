CC=gcc -Wall

build-all:
	make build-task1
	make build-task2

build-task1:
	$(CC) ./Task1/main.c -o ./Task1/main

build-task2:
	$(CC) ./Task2/master.c -o ./Task2/master
	$(CC) ./Task2/slave.c -o ./Task2/slave

run-task1:
	./Task2/main

run-task2:
	./Task2/master /tmp/myfifo 5
	./Task2/slave /tmp/myfifo 5
