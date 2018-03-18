.PHONY: all test

CC=gcc -Wall
REC_SIZE=100
AMOUNT=10

all:
	make build
	make test-full-suit

build:
	$(CC) main.c file_manager.h  file_manager.c time_full.h time_full.c -o main

test-full-suit:
	REC_SIZE=4
	AMOUNT=10
	make test
	REC_SIZE=512
	AMOUNT=10
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


build-dir-manager:
	$(CC) main_dir.c -o main_dir
	$(CC) main_nftw.c -o main_nftw

test-dir-manager:
	./main_dir . \> 2018-01-01
