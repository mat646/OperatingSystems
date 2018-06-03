//
// Created by Mateusz Sokol on 03.06.18.
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

char *bufor[MAX_SIZE];
pthread_t tid[MAX_SIZE];
int N, P, K, L;

int prod_index = 0, cons_index = 0;

pthread_mutex_t lock[MAX_SIZE];
pthread_cond_t cond[MAX_SIZE];

void int_handler(int signo)
{
    if(signo == SIGINT)
    {
        printf("CTRL+C\n");
        exit(0);
    }
}


int main() {

}