CC=gcc -Wall -O$(Opt)

all: build-all run-all

build-all: build-time build-static build-shared build-dynamic

run-all: test-static test-shared test-dynamic

build-static: build-objects-static build-libs-static build-main build-exec

build-objects-static: table_manager.c table_manager_static.c
	$(CC) -c table_manager.c table_manager_static.c

build-libs-static: table_manager.o table_manager_static.o
	ar rcs table_manager.a table_manager.o
	ar rcs table_manager_static.a table_manager_static.o

build-time: time_full.c
	$(CC) -c time_full.c

build-main: main.c
	$(CC) -c main.c

build-exec: main.o  table_manager.a table_manager_static.a
	$(CC) main.o time_full.o table_manager.a table_manager_static.a -o main

build-shared:
	$(CC) -fPIC -c table_manager.c table_manager_static.c
	$(CC) -shared -o table_manager.so table_manager.o
	$(CC) -shared -o table_manager_static.so table_manager_static.o
	$(CC)  main.c time_full.o table_manager.so table_manager_static.so -L. -Wl,-rpath=. -o main_shared

build-dynamic:
	$(CC) -fPIC -c table_manager.c table_manager_static.c
	$(CC) -shared -o table_manager.so table_manager.o
	$(CC) -shared -o table_manager_static.so table_manager_static.o
	$(CC) main_dynamic.c time_full.o -o main_dynamic -ldl

test-static:
	@echo "";
	@echo "######TESTING STATIC LIBRARY######";
	@echo "";
	./main 10000 10000 0 123
	./main 10000 10000 1 123

test-shared:
	@echo "";
	@echo "######TESTING SHARED LIBRARY######";
	@echo "";
	./main_shared 10000 10000 0 123
	./main_shared 10000 10000 1 123

test-dynamic:
	@echo "";
	@echo "######TESTING DYNAMIC LIBRARY######";
	@echo "";
	./main_dynamic 10000 10000 0 123
	./main_dynamic 10000 10000 1 123

writing-to-file:
	@echo "Writing to files...";
	make > ./results3a.txt
	make Opt=1 > ./results3b1.txt
	make Opt=2 > ./results3b2.txt
	make Opt=3 > ./results3b3.txt
	make Opt=s > ./results3bs.txt
	@echo "Finished";