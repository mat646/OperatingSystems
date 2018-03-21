.PHONY: all test

CC=gcc -Wall
REC_SIZE=4
AMOUNT=500

all:
	make build
	make test-first-suit

build:
	$(CC) main.c file_manager.h  file_manager.c time_full.h time_full.c -o main
	$(CC) main_dir.c -o main_dir
	$(CC) main_nftw.c -o main_nftw

test-first-suit:
	make test

test:
	make generate
	make copy-sys
	make sort-sys
	make copy-lib
	make sort-lib

generate:
	./main generate test $(REC_SIZE) $(AMOUNT)

copy-sys:
	./main copy test test2 $(REC_SIZE) $(AMOUNT) sys

copy-lib:
	./main copy test test3 $(REC_SIZE) $(AMOUNT) lib

sort-sys:
	./main sort test2 $(REC_SIZE) $(AMOUNT) sys

sort-lib:
	./main sort test3 $(REC_SIZE) $(AMOUNT) lib


test-second-suit:
	make set-up-dirs
	make test-dir-manager
	make test-nftw-manager

set-up-dirs:
	mkdir test_dir
	touch test_dir/file1
	echo testtesttesttest > test_dir/file1
	touch test_dir/file2
	echo zxvczxvcz > test_dir/file2
	mkdir test_dir/another_dir
	mkdir test_dir/yet_another_dir
	touch test_dir/another_dir/file3
	echo qwertyqwerty > test_dir/another_dir/file3
	ln -s test_dir/another_dir .
	touch test_dir/yet_another_dir/file4

test-dir-manager:
	./main_dir . \> 2018-01-01

test-nftw-manager:
	./main_nftw . \> 2018-01-01
