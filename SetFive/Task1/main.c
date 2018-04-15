//
// Created by Mateusz Sokol on 22.03.18.
//

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int fd[2];
int deep = 0;
char* array[100];

void execute(int level) {
    int argumentNumber = 1;
    char *token;
    token = strtok(array[level], " \n\t");
    char *params[4];
    while ((params[argumentNumber] = strtok(NULL, " \n\t")) != NULL) {
        argumentNumber++;
        if (argumentNumber >= 10) {
            printf("too many arguments");
            return;
        }
    };
    params[0] = token;
    params[argumentNumber+1] = NULL;

    int argumentNumber2 = 1;
    char *token2;
    token2 = strtok(array[level+1], " \n\t");
    char *params2[4];
    while ((params2[argumentNumber2] = strtok(NULL, " \n\t")) != NULL) {
        argumentNumber2++;
        if (argumentNumber >= 10) {
            printf("too many arguments");
            return;
        }
    };
    params2[0] = token2;
    params2[argumentNumber2+1] = NULL;

    int childpid;

    pipe(fd);
    if(childpid=fork()){
        //parent
        close(fd[1]);
        dup2(fd[0],STDIN_FILENO);
        execvp(params2[0],params2);
    }else{
        //child
        //write
        close(fd[0]);
        dup2(fd[1],STDOUT_FILENO);
        execvp(params[0], params);
    }
}

int main(int argc, char **argv) {

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

    char command[100];

    while (fgets(command, 100, file)) {
        deep = 0;
        printf("\n%s\n", command);

        char *single_command = strtok(command, "|");

        while (single_command != NULL) {
            array[deep] = single_command;
            deep++;
            single_command = strtok(NULL, "|");
        }

        int pid = vfork();
        if (pid == 0) {
            execute(0);
        } else {
            int status;
            wait(&status);
        }
    }
    fclose(file);
    return 0;
}
