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
int PID;
int is_in_queue = 0;
struct barber_shop* data;

void init_shm() {

    char *home = getenv("HOME");
    key_t key = ftok(home, SHM_ID);

    int memid;
    if ((memid = shmget(key, sizeof(struct barber_shop), 0666)) == -1) {
        perror("shmget");
        exit(1);
    }

    data = shmat(memid, NULL, 0);

}

int init_sem() {

    char *home = getenv("HOME");
    key_t key = ftok(home, SEM_ID);

    int semid = semget(key, 1, 0666);

    return semid;

}

int main(int argc, char **argv) {

    if (argc == 1 || argv[1] == NULL) {
        printf("Parametry wywolania:\n 1. liczba strzyzen\n");
        return 0;
    }

    int S;
    sscanf(argv[1], "%d", &S);

    PID = getpid();

    init_shm();

    int sem_id = init_sem();

    int sem_val;

    while (visits < S) {

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

                if(is_in_queue == 0) {
                    if (data->barber_state == 's' && data->queue_start == data->queue_end) {
                        data->barber_state = 'w';
                        printf("CLIENT %d: Waking up barber\n", PID);
                        printf("CLIENT %d: Sitting on chair\n", PID);
                        data->on_chair = PID;
                    } else if (data->barber_state == 'w' && data->on_chair == PID) {
                        printf("CLIENT %d: Finished, leaving\n", PID);
                        visits++;
                    } else if(data->queue_start - data->queue_end < data->queue_len) {
                        data->queue[++data->queue_start] = PID;
                        is_in_queue = data->queue_start;
                        printf("CLIENT %d: Waiting in queue\n", PID);
                    } else {
                        printf("CLIENT %d: Queue full, leaving\n", PID);
                    }
                } else {
                    if (data->queue[is_in_queue] == -1) {
                        printf("CLIENT %d: Sitting on chair\n", PID);
                        is_in_queue = 0;
                        data->on_chair = PID;
                        visits++;
                        printf("CLIENT %d: Finished, leaving\n", PID);
                    }
                }

                exit(0);
            }
        }

        sleep(1);

    }

    exit(0);
}