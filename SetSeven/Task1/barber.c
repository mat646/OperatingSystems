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

struct barber_shop* data;
char last_state = 's';

void init_shm(int N) {

    char *home = getenv("HOME");
    key_t key = ftok(home, SHM_ID);

    int memid;
    if ((memid = shmget(key, sizeof(struct barber_shop), IPC_CREAT | 0666)) == -1) {
        perror("shmget");
        exit(1);
    }

    data = shmat(memid, NULL, 0);

    data->barber_state = 's';
    data->queue_start = 0;
    data->queue_end = 0;
    data->queue_len = N;

}

int init_sem() {

    char *home = getenv("HOME");
    key_t key = ftok(home, SEM_ID);

    int semid = semget(key, 1, IPC_CREAT | 0666);

    struct sembuf *sops = (struct sembuf *) malloc(sizeof(struct sembuf));

    sops[0].sem_num = 0;
    sops[0].sem_op = 1;
    sops[0].sem_flg = 0;

    semop(semid, sops, 1);

    return semid;

}

int main(int argc, char **argv) {

    if (argc == 1 || argv[1] == NULL) {
        printf("Parametry wywolania:\n 1. liczba krzesel\n");
        return 0;
    }

    int N;
    sscanf(argv[1], "%d", &N);

    init_shm(N);

    int sem_id = init_sem();

    int sem_val;

    while (1) {

        if ((sem_val = semctl(sem_id, 0, GETVAL)) == -1) {
            perror("semctl: semctl failed");
            exit(1);
        }

        if(sem_val > 0) {

            int pid = vfork();
            if(pid == 0) {
                struct sembuf *sops = (struct sembuf *) malloc(sizeof(struct sembuf));

                sops[0].sem_num = 0;
                sops[0].sem_op = (short) (sops[0].sem_op - 1);
                sops[0].sem_flg = SEM_UNDO;

                semop(sem_id, sops, 1);

                if (data->queue_start == data->queue_end && data->barber_state == 's' && last_state == 's') {

                } else if(data->barber_state == 'w' && last_state == 's') {
                    printf("BARBER: Waking up\n");
                    last_state = 'w';
                    printf("BARBER: Starting work\n");
                } else if(data->barber_state == 'w' && last_state == 'w') {
                    printf("BARBER: Finished work\n");
                    data->on_chair = -1;
                    data->barber_state = 's';
                } else if(data->barber_state == 's'  && last_state == 'w' && data->queue_start == data->queue_end) {
                    last_state = 's';
                    printf("BARBER: Going to sleep\n");
                } else if(data->barber_state == 's'  && last_state == 'w') {
                    printf("BARBER: Inviting %d\n", data->queue[data->queue_end+1]);
                    data->on_chair = data->queue[data->queue_end+1];
                    data->queue[data->queue_end+1] = -1;
                    data->queue_end++;
                    data->barber_state = 'w';
                    printf("BARBER: Starting work\n");
                }

                exit(0);
            }

        }

        sleep(1);
    }

}