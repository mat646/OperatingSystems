CC=gcc -Wall

build:
	$(CC) main_dir_fork.c -o main_dir_fork
	$(CC) main.c -o main
	$(CC) main_with_restr.c time_full.h time_full.c -o main_with_restr
	$(CC) looper.c -o looper

test:
	./main_dir_fork . \> 2018-01-01
	./main example_file
	./main_with_restr example_file 100 100

run-looper:
	./main_with_restr looper_file 100 100
