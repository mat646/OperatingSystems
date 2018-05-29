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

char* bufor[MAX_SIZE];
pthread_t tid[MAX_SIZE];
int N, P, K, L;

int prod_index, cons_index;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *producer(void *x_void_ptr) {

    while (1) {
        pthread_mutex_lock(&lock);
        /* operacje na x... */
        printf("dawanie\n");
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&lock);
    }

    return 0;
}

void *consumer(void *x_void_ptr) {

    while (1) {
        pthread_mutex_lock(&lock);
        /* operacje na x... */
        printf("branie\n");
        pthread_cond_wait(&cond, &lock);
        pthread_mutex_unlock(&lock);
    }

    return 0;
}

int main(int argc, char **argv) {

    if (argc == 1 || argv[1] == NULL || argv[2] == NULL || argv[3] == NULL) {
        printf("Parametry wywolania:\n 1. rozmiar bufora\n 2. liczba producentow\n 3. liczba konsumentow\n");
        return 0;
    }

    sscanf(argv[1], "%d", &N);
    sscanf(argv[2], "%d", &P);
    sscanf(argv[3], "%d", &K);

    for (int i = 0; i < P; ++i) {
        if (pthread_create(&tid[i], NULL, producer, NULL)) {
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }
    }

    for (int i = 0; i < K; ++i) {
        if (pthread_create(&tid[P+i], NULL, consumer, NULL)) {
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }
    }

    sleep(N*K);

    return 0;
}