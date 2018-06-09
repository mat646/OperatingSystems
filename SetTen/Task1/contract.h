//
// Created by Mateusz Sokol on 08.06.18.
//

#ifndef OPERATINGSYSTEMS_CONTRACT_H
#define OPERATINGSYSTEMS_CONTRACT_H

typedef struct eval {
    int type; // 0 - log in, 1 - ping, 2 - operation
    char operand; // available: '+', '-', '*', '/'
    int arg1; // also result of operation
    int arg2;
} eval;

typedef struct msg {
    char name[100];
    eval eval1;
} msg;

#endif //OPERATINGSYSTEMS_CONTRACT_H
