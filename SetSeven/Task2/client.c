//
// Created by Mateusz Sokol on 01.05.18.
//

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <time.h>
#include <sys/time.h>
#include "contract.h"

int visits = 0;
int PID;
int is_in_queue = 0;
struct barber_shop* data;

void init_shm() {

    int memid;
    if ((memid = shm_open("/barber", O_RDWR, S_IRWXO | S_IRWXU | S_IRWXG)) == -1) {
        perror("shm_open");
        exit(1);
    }

    if (ftruncate(memid, sizeof(struct barber_shop)) == -1) {
        perror("ftruncate");
        exit(1);
    }

    data = mmap(NULL, sizeof(struct barber_shop),
                PROT_READ | PROT_WRITE, MAP_SHARED, memid, 0);

}

sem_t* init_sem() {

    sem_t *semid = sem_open("/sem", O_CREAT, 0666, 1);

    return semid;

}

char* my_clock() {
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime );
    return asctime(timeinfo);
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

    sem_t* sem_id = init_sem();

    int *sem_val = NULL;

    while (visits < S) {

        if ((sem_getvalue(sem_id, sem_val)) == -1) {
            perror("semctl: semctl failed");
            exit(1);
        }

        if(sem_val > 0) {

            int pid = vfork();
            if(pid == 0) {

                sem_wait(sem_id);

                if(is_in_queue == 0) {
                    if (data->barber_state == 's' && data->queue_start == data->queue_end) {
                        data->barber_state = 'w';
                        printf("%s CLIENT %d: Waking up barber\n", my_clock(), PID);
                        printf("%s CLIENT %d: Sitting on chair\n", my_clock(), PID);
                        data->on_chair = PID;
                    } else if (data->barber_state == 'w' && data->on_chair == PID) {
                        printf("%s CLIENT %d: Finished, leaving\n", my_clock(), PID);
                        visits++;
                    } else if(data->queue_start - data->queue_end < data->queue_len) {
                        data->queue[++data->queue_start] = PID;
                        is_in_queue = data->queue_start;
                        printf("%s CLIENT %d: Waiting in queue\n", my_clock(), PID);
                    } else {
                        printf("%s CLIENT %d: Queue full, leaving\n", my_clock(), PID);
                    }
                } else {
                    if (data->queue[is_in_queue] == -1) {
                        printf("%s CLIENT %d: Sitting on chair\n", my_clock(), PID);
                        is_in_queue = 0;
                        data->on_chair = PID;
                        visits++;
                        printf("%s CLIENT %d: Finished, leaving\n", my_clock(), PID);
                    }
                }

                sem_post(sem_id);

                exit(0);
            }
        }

        sleep(1);

    }

    exit(0);
}