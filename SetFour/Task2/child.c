//
// Created by Mateusz Sokol on 07.04.18.
//

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void user_signal(int signum);

int main() {
    signal(SIGUSR1, user_signal);
    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask, SIGUSR1);

    int wait_time = (rand() % 10);

    printf("PID: %d SLEEP: %d\n", getpid(), wait_time);
    fflush(stdout);
    sleep(wait_time);

    kill(getppid(), SIGUSR1);

    sigsuspend(&mask);

    return wait_time;
}

void user_signal(int signum) {
    kill(getppid(), SIGRTMIN + (rand() % (SIGRTMAX - SIGRTMIN)));
}