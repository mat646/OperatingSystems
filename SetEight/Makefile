.PHONY: all clean run

CC=gcc -Wall


all: main

main: main.c
	$(CC) $^ -o $@ -lpthread -lm

clean:
	rm main

run:
	./main 2 assets/cat.pgm assets/filter assets/out.pgm
