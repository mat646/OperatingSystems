.PHONY: all clean run

CC=gcc -Wall


all: main

main: main.c
	$(CC) $^ -o $@ -lpthread -lm

clean:
	rm main

run:
	./main 2 assets/cat.pgm assets/filter assets/out.pgm

multiple:
	./main 1 assets/cat.pgm assets/filter assets/out.pgm
	./main 2 assets/cat.pgm assets/filter assets/out.pgm
	./main 4 assets/cat.pgm assets/filter assets/out.pgm
	./main 8 assets/cat.pgm assets/filter assets/out.pgm
	./main 1 assets/cat.pgm assets/filter2 assets/out.pgm
	./main 2 assets/cat.pgm assets/filter2 assets/out.pgm
	./main 4 assets/cat.pgm assets/filter2 assets/out.pgm
	./main 8 assets/cat.pgm assets/filter2 assets/out.pgm
