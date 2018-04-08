CC=gcc -Wall

build-all:
	make build-task1
	make build-task2
	make build-task3

build-task1:
	$(CC) ./Task1/main.c -o ./Task1/main
	$(CC) ./Task1/main_fork.c -o ./Task1/main_fork
	chmod a+x ./Task1/date.sh

build-task2:
	$(CC) ./Task2/child.c -o ./Task2/child
	$(CC) ./Task2/main.c -o ./Task2/main

build-task3:
	$(CC) ./Task3/main.c -o ./Task3/main

run-task1:
	./Task1/main

run-task1-fork:
	./Task1/main_fork

run-task2:
	./Task2/main 10 4

run-task3-type1:
	./Task3/main 8 1

run-task3-type2:
	./Task3/main 8 2

run-task3-type3:
	./Task3/main 8 3
