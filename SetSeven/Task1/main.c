//
// Created by Mateusz Sokol on 01.05.18.
//

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>

key_t key;

int main(int argc, char **argv) {

    if (argc == 1 || argv[1] == NULL) {
        printf("Parametry wywolania:\n 1. liczba klientow\n 2. liczba strzyzen\n");
        return 0;
    }

    int N, S;
    sscanf(argv[1], "%d", &N);
    sscanf(argv[2], "%d", &S);


    int barberPID = 0;//fork();
    if (barberPID == 0) {

        //create barber and semaphore

        char *home = getenv("HOME");
        key = ftok(home, 2200);

        int memid;
        if ((memid = shmget(key, 1024, IPC_CREAT | 0666)) == -1) {
            perror("shmget b");
            exit(1);
        }

        void* data = shmat(memid, NULL, 0);
        if (data == (char *)(-1)) {
            perror("shmat b");
            exit(1);
        }

        strncpy(data, "dzialaaaa", 1024);

        //while () {

        //}

    //} else {

        sleep(3);

        for (int i = 0; i < N; ++i) {
            int pid = fork();

            if (pid == 0) {
                //client
                int visits = 0;

                int memid;
                if ((memid = shmget(key, 1024, 0666)) == -1) {
                    perror("shmget c");
                    exit(1);
                }

                void* data = shmat(memid, NULL, 0);
                if (data == (char *)(-1)) {
                    perror("shmat");
                    exit(1);
                }

                char xd[1024];

                strncpy(xd, data, 1024);

                printf("%s a", xd);

                // while (visits < S)
                //semaphore

                exit(0);

            }
        }
    }

}