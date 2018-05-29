.PHONY: all clean run

CC=gcc -Wall


all: main

main: main.c
	$(CC) $^ -o $@ -lpthread -lm

clean:
	rm main

run:
	./main 5 2 2
