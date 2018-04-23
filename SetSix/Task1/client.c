//
// Created by Mateusz Sokol on 21.04.18.
//

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
#include "contract.h"

int privateQueue = -1;

void sig_int(int sig){
    _exit(0);
}

void delete_queue(void){
    if(privateQueue > -1){
        msgctl(privateQueue, IPC_RMID, NULL);
        printf("Client's main_queue deleted successfully!\n");
    }
}

int main() {
    atexit(delete_queue);

    signal(SIGINT, sig_int);

    char *home = getenv("HOME");

    key_t key = ftok(home, QUEUE_ID);
    int publicQueue = msgget(key, 0);


    key_t privateKey = ftok(home, getpid());
    privateQueue = msgget(privateKey, IPC_CREAT | IPC_EXCL | 0666);

    //register client
    msg msg;
    msg.type = LOGIN;
    msg.pid = getpid();
    msg.key = privateKey;
    if(msgsnd(publicQueue, &msg, MSG_SIZE, 0) == -1) {
        printf("Unable to register client.\n");
        _exit(1);
    }

    while (1) {
        printf("Press key (m - mirror, + - add, - - del, * - mul, t - time, e - end)\n");
        char action;

        scanf(" %c", &action);

        switch (action) {
            case 'm':
                msg.type = MIRROR;
                strcpy(msg.text, "This is tutorialspoint.com\n");
                break;
            case '+':
                msg.type = CALC;
                strcpy(msg.text, "ADD 2 4\n");
                break;
            case '-':
                msg.type = CALC;
                strcpy(msg.text, "SUB 13 9\n");
                break;
            case '*':
                msg.type = CALC;
                strcpy(msg.text, "MUL 7 9\n");
                break;
            case 't':
                msg.type = TIME;
                break;
            case 'e':
                msg.type = END;
                break;
            default:
                break;
        }

        if (action == 'e') {
            msgsnd(publicQueue, &msg, MSG_SIZE, 0);
            _exit(0);
        }

        msgsnd(publicQueue, &msg, MSG_SIZE, 0);
        msgrcv(privateQueue, &msg, MSG_SIZE, 0, 0);
        printf("%s\n", msg.text);
    }

}