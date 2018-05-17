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
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include "contract.h"

struct barber_shop* data;
char last_state = 's';
sem_t* semid;

void init_shm(int N) {

    int memid;
    if ((memid = shm_open("/barber", O_CREAT | O_RDWR, S_IRWXO | S_IRWXU | S_IRWXG)) == -1) {
        perror("shm_open");
        exit(1);
    }

    if (ftruncate(memid, sizeof(struct barber_shop)) == -1) {
        perror("ftruncate");
        exit(1);
    }

    data = mmap(NULL, sizeof(struct barber_shop),
                PROT_READ | PROT_WRITE, MAP_SHARED, memid, 0);

    data->barber_state = 's';
    data->queue_start = 0;
    data->queue_end = 0;
    data->queue_len = N;

}

sem_t* init_sem() {

    sem_t *semid = sem_open("/sem", O_CREAT, 0660, 1);

    return semid;
}

void close_barber(int signo) {
    printf("Closing barber shop.\n");
    sem_close(semid);
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
        printf("Parametry wywolania:\n 1. liczba krzesel\n");
        return 0;
    }

    if (signal(SIGINT, close_barber) == SIG_ERR) {
        printf("Cannot proceed SIGINT\n");
        exit(1);
    }

    int N;
    sscanf(argv[1], "%d", &N);

    init_shm(N);

    sem_t* sem_id = init_sem();
    semid = sem_id;

    int *sem_val = NULL;

    while (1) {

        if ((sem_getvalue(sem_id, sem_val)) == -1) {
            perror("semctl: semctl failed");
            exit(1);
        }

        if(*sem_val > 0) {

            int pid = vfork();
            if(pid == 0) {

                sem_wait(sem_id);

                if (data->queue_start == data->queue_end && data->barber_state == 's' && last_state == 's') {

                } else if(data->barber_state == 'w' && last_state == 's') {
                    printf("%s BARBER: Waking up\n", my_clock());
                    last_state = 'w';
                    printf("%s BARBER: Starting work\n", my_clock());
                } else if(data->barber_state == 'w' && last_state == 'w') {
                    printf("%s BARBER: Finished work\n", my_clock());
                    data->on_chair = -1;
                    data->barber_state = 's';
                } else if(data->barber_state == 's'  && last_state == 'w' && data->queue_start == data->queue_end) {
                    last_state = 's';
                    printf("%s BARBER: Going to sleep\n", my_clock());
                } else if(data->barber_state == 's'  && last_state == 'w') {
                    printf("%s BARBER: Inviting %d\n", my_clock(), data->queue[data->queue_end+1]);
                    data->on_chair = data->queue[data->queue_end+1];
                    data->queue[data->queue_end+1] = -1;
                    data->queue_end++;
                    data->barber_state = 'w';
                    printf("%s BARBER: Starting work\n", my_clock());
                }

                sem_post(sem_id);

                exit(0);
            }

        }

        sleep(1);
    }

}