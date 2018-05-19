.PHONY: all clean

CC=gcc -Wall


all: main

main: main.c
	$(CC) $^ -o $@ -lpthread -lm

clean:
	rm main
