//
// Created by Mateusz Sokol on 29.03.18.
//

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <time.h>
#include "time_full.h"

void set_limits(rlim_t time_restr, rlim_t mem_restr);

int main(int argc, char **argv) {

    if (argc == 1 || argv[1] == NULL || argv[2] == NULL || argv[3] == NULL) {
        printf("Parametry wywolania:\n 1. ścieżka do pliku\n 2. ograniczenie czasu\n 3. ograniczenie pamięci\n");
        return 0;
    }

    char fileName[40];
    int timeConstraint, memConstraint;
    sscanf(argv[1], "%s", fileName);
    sscanf(argv[2], "%d", &timeConstraint);
    sscanf(argv[3], "%d", &memConstraint);

    memConstraint *= 1024 * 1024;

    FILE *file = fopen(fileName, "r");
    if (!file) {
        printf("Invalid path");
        return 1;
    }

    struct rusage prev_usage;
    getrusage(RUSAGE_CHILDREN, &prev_usage);

    char command[100];
    char *params[100];
    while (fgets(command, 100, file)) {
        int argumentNumber = 1;
        char *token;
        token = strtok(command, " \n\t");

        while ((params[argumentNumber] = strtok(NULL, " \n\t")) != NULL) {
            argumentNumber++;
            if (argumentNumber >= 10) {
                printf("too many arguments");
                return 1;
            }
        };
        params[0] = token;

        Time time1;

        pid_t pid = vfork();
        if (pid == 0) {
            set_limits((rlim_t) timeConstraint, (rlim_t) memConstraint);

            time1 = start();

            execvp(token, params);
            _exit(1);
        }
        int status;
        wait(&status);
        if (status) {
            printf("Error while executing %s\n", params[0]);
            return 1;
        } else {
            end(&time1);
            print(time1, params[0]);
            printf("\n");
        }
    }
    fclose(file);
    return 0;
}

void set_limits(rlim_t time_restr, rlim_t mem_restr) {

    struct rlimit rlim_cpu;
    rlim_cpu.rlim_cur = 1;
    rlim_cpu.rlim_max = time_restr;
    if (setrlimit(RLIMIT_CPU, &rlim_cpu) == -1) {
        perror("Set cpu limit failed");
        _exit(1);
    }

    struct rlimit rlim_mem;
    rlim_mem.rlim_cur = mem_restr / 2;
    rlim_mem.rlim_max = mem_restr;
    if (setrlimit(RLIMIT_AS, &rlim_mem) == -1) {
        perror("Set mem limit failed");
        _exit(1);
    }

}