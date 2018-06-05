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
#include <string.h>

char *bufor[MAX_SIZE];
pthread_t tid[MAX_SIZE];
int N, P, K, L;

volatile int prod_index = 0, cons_index = 0;
sem_t array_sem;
sem_t array_sem2;
FILE *file;

pthread_mutex_t lock[MAX_SIZE];
pthread_mutex_t lock2[MAX_SIZE];
pthread_cond_t cond[MAX_SIZE];
pthread_cond_t cond2[MAX_SIZE];
pthread_mutex_t prod_index_get = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cons_index_get = PTHREAD_MUTEX_INITIALIZER;

void int_handler(int signo) {
    if (signo == SIGINT) {
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

        pthread_mutex_lock(&lock[index]);
        if (bufor[index] != NULL) {
            pthread_cond_wait(&cond2[index], &lock[index]);
        }
        printf("%d dawanie przez %ld\n", index, pthread_self());
        int len = rand() % 20;
        bufor[index] = (char *) malloc(len * sizeof(char));
        fgets(bufor[index], len, file);


        pthread_cond_broadcast(&cond[index]);
        pthread_mutex_unlock(&lock[index]);
    }

}

void *consumer(void *x_void_ptr) {

    while (1) {

        pthread_mutex_lock(&cons_index_get);
        int index = cons_index++;
        cons_index = cons_index % N;
        pthread_mutex_unlock(&cons_index_get);

        pthread_mutex_lock(&lock2[index]);
        if (bufor[index] == NULL) {
            pthread_cond_wait(&cond[index], &lock2[index]);
        }

        if (bufor[index] != NULL) {
            printf("%d branie przez %ld\n", index, pthread_self());
            if (strlen(bufor[index]) == L) {
                for (int j = 0; j < strlen(bufor[index]); ++j) {
                    printf("%c", bufor[index][j]);
                }
            }
            printf("\n");
        }
        bufor[index] = NULL;

        pthread_cond_broadcast(&cond2[index]);
        pthread_mutex_unlock(&lock2[index]);
    }

}

int main(int argc, char **argv) {

    if (argc == 1 || argv[1] == NULL || argv[2] == NULL || argv[3] == NULL || argv[4] == NULL) {
        printf("Parametry wywolania:\n 1. rozmiar bufora\n 2. liczba producentow\n 3. liczba konsumentow\n 4. poszukiwana dlugosc\n");
        return 0;
    }

    sscanf(argv[1], "%d", &N);
    sscanf(argv[2], "%d", &P);
    sscanf(argv[3], "%d", &K);
    sscanf(argv[4], "%d", &L);

    file = fopen("pan_tadeusz", "r");

    if (sem_init(&array_sem, 0, 1) < 0) printf("Couldnt make semaphore.\n");
    if (sem_init(&array_sem2, 0, 1) < 0) printf("Couldnt make semaphore.\n");

    for (int i = 0; i < N; ++i) {
        pthread_mutex_init(&lock[i], NULL);
        pthread_mutex_init(&lock2[i], NULL);
        pthread_cond_init(&cond[i], NULL);
        pthread_cond_init(&cond2[i], NULL);
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