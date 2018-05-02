//
// Created by Mateusz Sokol on 01.05.18.
//

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "contract.h"

void init_shm(int N) {

    char *home = getenv("HOME");
    key_t key = ftok(home, SHM_ID);

    int memid;
    if ((memid = shmget(key, sizeof(struct barber_shop), IPC_CREAT | 0666)) == -1) {
        perror("shmget");
        exit(1);
    }

    struct barber_shop* data = shmat(memid, NULL, 0);
    if (data == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }

    data->barber_state = 's';
    data->queue_start = 0;
    data->queue_end = 0;
    data->queue_len = N;

}

void init_sem() {

    char *home = getenv("HOME");
    key_t key = ftok(home, SEM_ID);

    int semid = semget(key, 1, IPC_CREAT | 0666);

    struct sembuf *sops = (struct sembuf *) malloc(sizeof(struct sembuf));

    sops[0].sem_num = 0;
    sops[0].sem_op = 1;
    sops[0].sem_flg = 0;

    semop(semid, sops, 1);

}

int main(int argc, char **argv) {

    if (argc == 1 || argv[1] == NULL) {
        printf("Parametry wywolania:\n 1. liczba krzesel\n");
        return 0;
    }

    int N;
    sscanf(argv[1], "%d", &N);

    init_shm(N);

    init_sem();

    while (1) {

        //check queue

        sleep(1);

    }

}