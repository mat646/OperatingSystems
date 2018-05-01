//
// Created by Mateusz Sokol on 01.05.18.
//

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {

    if (argc == 1 || argv[1] == NULL) {
        printf("Parametry wywolania:\n 1. liczba strzyzen\n");
        return 0;
    }

    int S;
    sscanf(argv[1], "%d", &S);

}