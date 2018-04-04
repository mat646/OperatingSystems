//
// Created by Mateusz Sokol on 22.03.18.
//

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

    if (argc == 1 || argv[1] == NULL) {
        printf("Parametry wywolania:\n 1. ścieżka do pliku\n");
        return 0;
    }

    char fileName[40];
    sscanf(argv[1], "%s", fileName);

    FILE* file = fopen(fileName, "r");
    if (!file) {
        printf("Invalid path");
        return 1;
    }

    char command[100];
    char *params[100];
    while(fgets(command, 100, file)){
        int argumentNumber = 1;
        char *token;
        token = strtok(command, " \n\t");

        while((params[argumentNumber] = strtok(NULL, " \n\t")) != NULL){
            argumentNumber++;
            if(argumentNumber >= 10){
                printf("too many arguments");
                return 1;
            }
        };
        params[0] = token;

        pid_t pid = vfork();
        if(pid == 0) {
            execvp(token, params);
            _exit(1);
        }
        int status;
        wait(&status);
        if (status) {
            printf("Error while executing");
            return 1;
        }
    }
    fclose(file);
    return 0;
}