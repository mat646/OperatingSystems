//
// Created by Mateusz Sokol on 22.04.18.
//

#ifndef OPERATINGSYSTEMS_CONTRACT_H
#define OPERATINGSYSTEMS_CONTRACT_H

#define QUEUE_ID 100
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

typedef enum msg_type{
    LOGIN = 1, MIRROR = 2, CALC = 3, TIME = 4, END = 5
} msg_type;

typedef struct message {
    long type;
    pid_t pid;
    key_t key;
    char text[100];
} msg;

const size_t MSG_SIZE = sizeof(msg) - sizeof(long);

#endif //OPERATINGSYSTEMS_CONTRACT_H
