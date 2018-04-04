CC=gcc -Wall

build:
	$(CC) main_dir_fork.c -o main_dir_fork
	$(CC) main.c -o main
	$(CC) main_with_restr.c -o main_with_restr

test:
	./main_dir_fork . \> 2018-01-01
	./main example_file
	./main_with_restr example_file 10 10

