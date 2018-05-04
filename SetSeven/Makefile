.PHONY: all clean

CC=gcc -Wall


all: Task1/main Task1/client Task1/barber Task2/main Task2/client Task2/barber

Task1/main: Task1/main.c Task1/contract.h
	ipcrm --all=shm
	ipcrm --all=sem
	$(CC) $^ -o $@

Task1/client: Task1/client.c Task1/contract.h
	$(CC) $^ -o $@

Task1/barber: Task1/barber.c Task1/contract.h
	$(CC) $^ -o $@

Task2/main: Task2/main.c Task2/contract.h
	ipcrm --all=shm
	ipcrm --all=sem
	$(CC) $^ -o $@

Task2/client: Task2/client.c Task2/contract.h
	$(CC) $^ -o $@ -lrt -pthread

Task2/barber: Task2/barber.c Task2/contract.h
	$(CC) $^ -o $@ -lrt -pthread

clean:
	rm Task1/barber Task1/client Task1/main
	rm Task2/barber Task2/client Task2/main
