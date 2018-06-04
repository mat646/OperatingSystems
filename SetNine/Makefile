.PHONY: all clean run

CC=gcc -Wall


all: main_a main_b

main_a: main_a.c
	$(CC) $^ -o $@ -lpthread -lm

main_b: main_b.c
	$(CC) $^ -o $@ -lrt -lpthread -lm

clean:
	ipcrm --all=sem
	rm main_a main_b

run-a:
	./main_a 5 2 2 9

run-b:
	./main_b 5 2 2 9
