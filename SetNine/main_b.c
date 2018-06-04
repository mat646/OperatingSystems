//
// Created by Mateusz Sokol on 26.05.18.
//

#define _GNU_SOURCE
#define MAX_SIZE 1000

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <sys/param.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <fcntl.h>

char *bufor[MAX_SIZE];
pthread_t tid[MAX_SIZE];
int N, P, K, L;

pthread_mutex_t prod_index_get = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cons_index_get = PTHREAD_MUTEX_INITIALIZER;

volatile int prod_index = 0, cons_index = 0;
sem_t array_sem;
sem_t array_sem2;

sem_t* semid[MAX_SIZE];
sem_t* semid2[MAX_SIZE];

sem_t* init_sem(char* name) {

    printf("%s\n", name);
    sem_t *semid = sem_open(name, O_CREAT, 0660, 1);

    return semid;
}

sem_t* init_sem2(char* name) {

    printf("%s\n", name);
    sem_t *semid = sem_open(name, O_CREAT, 0660, 0);

    if (semid == SEM_FAILED) {
        printf("err%s\n", name);
    }

    return semid;
}

void int_handler(int signo)
{
    if(signo == SIGINT)
    {
        printf("CTRL+C\n");
        exit(0);
    }
}

void *producer(void *x_void_ptr) {

    while (1) {

        pthread_mutex_lock(&prod_index_get);
        int index = prod_index++;
        prod_index = prod_index % N;
        pthread_mutex_unlock(&prod_index_get);

        printf("xd\n");
        sem_wait(semid[index]);
        printf("%d dawanie przez %ld\n", index, pthread_self());
        int len = rand() % N + 5;
        bufor[index] = (char *) malloc(len * sizeof(char));
        for (int j = 0; j < len; ++j) {
            bufor[index][j] = (char) ((rand() % 24) + 97);
        }

        sem_post(semid2[index]);
    }

}

void *consumer(void *x_void_ptr) {

    while (1) {

        pthread_mutex_lock(&cons_index_get);
        int index = cons_index++;
        cons_index = cons_index % N;
        pthread_mutex_unlock(&cons_index_get);

        sem_wait(semid2[index]);
        printf("xd\n");
        printf("%d branie przez %ld\n", index, pthread_self());
        for (int j = 0; j < 3; ++j) {
            printf("%c", bufor[index][j]);
        }
        printf("\n");
        bufor[index] = NULL;

        sem_post(semid[index]);
    }

}

int main(int argc, char **argv) {

    if (sem_init(&array_sem, 0, 1) < 0) printf("Couldnt make semaphore.\n");
    if (sem_init(&array_sem2, 0, 1) < 0) printf("Couldnt make semaphore.\n");

    if (argc == 1 || argv[1] == NULL || argv[2] == NULL || argv[3] == NULL) {
        printf("Parametry wywolania:\n 1. rozmiar bufora\n 2. liczba producentow\n 3. liczba konsumentow\n");
        return 0;
    }

    sscanf(argv[1], "%d", &N);
    sscanf(argv[2], "%d", &P);
    sscanf(argv[3], "%d", &K);

    for (int j = 0; j < N; ++j) {
        char name[20];
        sprintf(name, "/sem%d", j);
        sem_t *semid1 = init_sem(name);
        semid[j] = semid1;
        sprintf(name, "/sem2%d", j);
        sem_t *semid3 = init_sem2(name);
        semid2[j] = semid3;
    }

    for (int i = 0; i < K; ++i) {
        if (pthread_create(&tid[i], NULL, consumer, NULL)) {
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }
    }

    for (int i = 0; i < P; ++i) {
        if (pthread_create(&tid[K + i], NULL, producer, NULL)) {
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }
    }

    sleep(1);
    //sleep(N * K);

    return 0;
}