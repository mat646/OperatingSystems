//
// Created by Mateusz Sokol on 22.03.18.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

#define _GNU_SOURCE
#define MAX_LEN 100

int pipes[2][2];

int main(int argc, char *argv[]) {

    if (argc == 1 || argv[1] == NULL) {
        printf("Parametry wywolania:\n 1. ścieżka do pliku\n");
        return 0;
    }

    char fileName[40];
    sscanf(argv[1], "%s", fileName);

    FILE *file = fopen(fileName, "r");
    if (!file) {
        printf("Invalid path");
        return 1;
    }

    char command[MAX_LEN];
    char *arg_arr[MAX_LEN], *cmd_arr[MAX_LEN];
    int deep = 0, num_args = 0, num_lines = 0;

    while (fgets(command, MAX_LEN, file)) {
        num_lines++;
        deep = 0;

        cmd_arr[deep] = strtok(command, "|\n");
        deep++;
        while ((cmd_arr[deep] = strtok(NULL, "|\n")) != NULL){
            deep++;
        }
        if (!deep) continue;

        int i;
        for (i = 0; i < deep; i++) {
            num_args = 0;
            arg_arr[num_args] = strtok( cmd_arr[i], " \t\n");
            num_args++;

            while ((arg_arr[num_args] = strtok(NULL, " \t\n")) != NULL) {
                num_args++;
            }
            if (!num_args) continue;

            if (i > 1) {
                close(pipes[i % 2][0]);
                close(pipes[i % 2][1]);
            }
            pipe(pipes[i % 2]);

            pid_t pid = fork();
            if (pid == 0) {
                if (i < deep - 1) {
                    close(pipes[i % 2][0]);
                    dup2(pipes[i % 2][1], 1);
                }
                if (i != 0) {
                    close(pipes[(i + 1) % 2][1]);
                    dup2(pipes[(i + 1) % 2][0], 0);
                }
                execvp(arg_arr[0], arg_arr);
            }
        }
        close(pipes[i % 2][0]);
        close(pipes[i % 2][1]);
        while (wait(NULL)) {
            if (errno == ECHILD) {
                break;
            }
        }
    }
    fclose(file);
    return 0;
}