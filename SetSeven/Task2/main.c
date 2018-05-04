//
// Created by Mateusz Sokol on 01.05.18.
//

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>
#include "contract.h"

int main(int argc, char **argv) {

    if (argc == 1 || argv[1] == NULL || argv[2] == NULL || argv[3] == NULL) {
        printf("Parametry wywolania:\n 1. liczba klientow\n 2. liczba strzyzen\n 3. liczba miejsc\n");
        return 0;
    }

    int N;
    sscanf(argv[1], "%d", &N);

    pid_t pid1 = fork();
    if (pid1 == 0) {
        execl("./Task1/barber", "./Task1/barber", argv[3], NULL);
        exit(0);
    }

    sleep(3);

    for (int i = 0; i < N; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            execl("./Task1/client", "./Task1/client", argv[2], NULL);
            exit(0);
        }
    }

}