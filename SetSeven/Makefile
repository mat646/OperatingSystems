.PHONY: all

CC=gcc -Wall


all: Task1/main Task1/client Task1/barber

Task1/main: Task1/main.c Task1/contract.h
	ipcrm --all=shm
	ipcrm --all=sem
	$(CC) $^ -o $@

Task1/client: Task1/client.c Task1/contract.h
	$(CC) $^ -o $@

Task1/barber: Task1/barber.c Task1/contract.h
	$(CC) $^ -o $@
