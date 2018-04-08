//
// Created by Mateusz Sokol on 07.04.18.
//

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <memory.h>
#include <signal.h>
#include <unistd.h>

pid_t pid;

int L, type, cycle = 0;

void parent_service(int signum);

void usr1_service(int signum, siginfo_t *info, void *context);

void usr2_service(int signum);

int main(int argc, char *argv[]) {
    if (argc == 1 || argv[1] == NULL || argv[2] == NULL) {
        printf("Parametry wywolania:\n 1. liczba zapytan\n 2. typ\n");
        return 0;
    }

    sscanf(argv[1], "%d", &L);
    sscanf(argv[2], "%d", &type);

    pid = fork();

    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_SIGINFO;

    if (pid == 0) {
        action.sa_sigaction = usr1_service;
        sigaction(SIGUSR1, &action, NULL);
        sigaction(SIGRTMIN, &action, NULL);
        sigaction(SIGRTMAX, &action, NULL);

        signal(SIGUSR2, &usr2_service);

        while (1) {
            sleep(1);
        }

    } else {
        sleep(1);

        if (type == 0 || type == 1) {
            signal(SIGUSR1, &parent_service);

            for (int i = 0; i < L; ++i) {
                sleep(1);
                kill(pid, SIGUSR1);
            }
            sleep(1);
            kill(pid, SIGUSR2);

        } else {
            signal(SIGRTMIN, &parent_service);

            kill(pid, SIGRTMIN);

            while (1) {
                sleep(1);
            }

        }

    }

}

void parent_service(int signum) {
    printf("PARENT: SIGUSR1 from PID: %d\n", pid);
    fflush(stdout);

    if (type == 3) {
        if (cycle == L) {
            kill(pid, SIGRTMAX);
            waitpid(pid, NULL, 0);
            _exit(0);
        } else {
            kill(pid, SIGRTMIN);
            cycle++;
        }
    }
}

void usr1_service(int signum, siginfo_t *info, void *context) {

    if (signum == SIGRTMIN) {
        kill(info->si_pid, SIGRTMIN);
        printf("CHILD: SIGRTMIN\n");
    } else if (signum == SIGRTMAX) {
        printf("CHILD: SIGRTMAX\n");
        _exit(0);
    } else {
        printf("CHILD: SIGUSR1 from PID: %d\n", info->si_pid);
        fflush(stdout);
        kill(info->si_pid, SIGUSR1);
    }

}

void usr2_service(int signum) {
    printf("END\n");
    _exit(0);
}
