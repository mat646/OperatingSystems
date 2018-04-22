//
// Created by Mateusz Sokol on 22.04.18.
//

#ifndef OPERATINGSYSTEMS_CONTRACT_H
#define OPERATINGSYSTEMS_CONTRACT_H

#define PROJECT_ID 100
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>
#include <time.h>
#include <signal.h>

typedef enum msg_type{
    LOGIN = 1, MIRROR = 2, CALC = 3, TIME = 4, END = 5, INIT = 6
} mtype;

typedef struct message {
    long myType;
    pid_t pid;
    char text[4096];
} msg;

const size_t MSG_SIZE = sizeof(msg) - sizeof(long);

#endif //OPERATINGSYSTEMS_CONTRACT_H
