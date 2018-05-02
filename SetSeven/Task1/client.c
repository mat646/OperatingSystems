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

int visits = 0;
int pid;
struct barber_shop* data;

void init_shm() {

    char *home = getenv("HOME");
    key_t key = ftok(home, SHM_ID);

    int memid;
    if ((memid = shmget(key, sizeof(struct barber_shop), 0666)) == -1) {
        perror("shmget c");
        exit(1);
    }

    data = shmat(memid, NULL, 0);
    if (data == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }

}

int init_sem() {

    char *home = getenv("HOME");
    key_t key = ftok(home, SEM_ID);

    int semid = semget(key, 1, 0666);

    struct sembuf *sops = (struct sembuf *) malloc(sizeof(struct sembuf));

    return semid;

}


int main(int argc, char **argv) {

    if (argc == 1 || argv[1] == NULL) {
        printf("Parametry wywolania:\n 1. liczba strzyzen\n");
        return 0;
    }

    int S;
    sscanf(argv[1], "%d", &S);

    pid = getpid();

    init_shm();

    int sem_id = init_sem();

    int sem_val;

    while (visits < S) {

        if ((sem_val = semctl(sem_id, 0, GETVAL)) == -1) {
            perror("semctl: semctl failed");
            exit(1);
        }

        if(sem_val > 0) {

            struct sembuf *sops = (struct sembuf *) malloc(sizeof(struct sembuf));

            sops[0].sem_num = 0; /* We only use one track */
            sops[0].sem_op = (short) (sops[0].sem_op - 1); /* wait for semaphore flag to become zero */
            sops[0].sem_flg = SEM_UNDO; /* take off semaphore asynchronous  */

            int state = semop(sem_id, sops, 1);

            printf("%c state\n", data->barber_state);

            sleep(1);

            exit(0);

        }

        sleep(1);

    }

    exit(0);

}