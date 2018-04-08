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

int N, K, n, k;
pid_t child_list[1000];
pid_t waiting_list[1000];

int get_child_pid(pid_t pid);

void remove_child(pid_t pid);

void int_service(int signum, siginfo_t *info, void *context);

void usr_service(int signum, siginfo_t *info, void *context);

void child_service(int signum, siginfo_t *info, void *context);

void real_time_service(int signum, siginfo_t *info, void *context);

int main(int argc, char *argv[]) {
    if (argc == 1 || argv[1] == NULL || argv[2] == NULL) {
        printf("Parametry wywolania:\n 1. liczba potomkow\n 2. liczba K\n");
        return 0;
    }

    sscanf(argv[1], "%d", &N);
    sscanf(argv[1], "%d", &K);

    n = 0;
    k = 0;

    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_SIGINFO;

    action.sa_sigaction = int_service;
    sigaction(SIGINT, &action, NULL);

    action.sa_sigaction = usr_service;
    sigaction(SIGUSR1, &action, NULL);

    action.sa_sigaction = child_service;
    sigaction(SIGCHLD, &action, NULL);

    for (int i = SIGRTMIN; i <= SIGRTMAX; i++) {
        action.sa_sigaction = real_time_service;
        sigaction(i, &action, NULL);
    }

    for (int i = 0; i < N; i++) {
        pid_t pid = vfork();
        if (pid == 0) {
            execl("./Task2/child", "./Task2/child", NULL);
        } else {
            child_list[n++] = pid;
        }
    }

    while (1) {

    }

}

int get_child_pid(pid_t pid) {
    for (int i = 0; i < N; i++)
        if (child_list[i] == pid) return i;
    return -1;
}

void remove_child(pid_t pid) {
    for (int i = 0; i < N; i++)
        if (child_list[i] == pid) {
            child_list[i] = -1;
            return;
        }
}

void int_service(int signum, siginfo_t *info, void *context) {
    printf("PARENT: Received SIGINT\n");

    for (int i = 0; i < N; i++)
        if (child_list[i] != -1) {
            kill(child_list[i], SIGKILL);
            waitpid(child_list[i], NULL, 0);
        }

    _exit(0);
}

void usr_service(int signum, siginfo_t *info, void *context) {
    printf("PARENT: SIGUSR1 from PID: %d\n", info->si_pid);

    if (get_child_pid(info->si_pid) == -1) {
        return;
    }

    if (k >= K) {
        kill(info->si_pid, SIGUSR1);
        waitpid(info->si_pid, NULL, 0);
    } else {
        waiting_list[k++] = info->si_pid;
        if (k >= K) {
            for (int i = 0; i < K; i++) {
                if (waiting_list[i] > 0) {
                    kill(waiting_list[i], SIGUSR1);
                    waitpid(waiting_list[i], NULL, 0);
                }
            }
        }
    }
}

void child_service(int signum, siginfo_t *info, void *context) {
    printf("PARENT: Child %d terminated: %d\n", info->si_pid, info->si_status);
    n = n-1;
    if (n == 0) {
        _exit(0);
    }
    remove_child(info->si_status);
}

void real_time_service(int signum, siginfo_t *info, void *context) {
    printf("PARENT: SIGRT: SIGMIN+%i, from PID: %d\n", signum - SIGRTMIN, info->si_pid);
}
