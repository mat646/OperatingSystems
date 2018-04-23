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
#include <mqueue.h>
#include "contract.h"

int privateQueue = -1;

void sig_int(int sig){
    _exit(0);
}

void delete_queue(void){
    if(privateQueue > -1){
        mq_close(privateQueue);
        printf("Queue deleted!\n");
    }
}

int main() {
    atexit(delete_queue);

    signal(SIGINT, sig_int);

    char *home = getenv("HOME");

    int publicQueue = mq_open("/server", O_WRONLY);

    struct mq_attr posixAttr;
    posixAttr.mq_maxmsg = 9;
    posixAttr.mq_msgsize = MSG_SIZE;

    key_t privateKey = ftok(home, getpid());
    privateQueue = mq_open("/client", O_RDONLY | O_CREAT | O_EXCL, 0666, &posixAttr);

    //register client
    msg msg;
    msg.type = LOGIN;
    msg.pid = getpid();
    msg.key = privateKey;
    if(mq_send(publicQueue, (char*) &msg, MSG_SIZE, 1) == -1) {
        printf("Unable to register.\n");
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
            mq_send(publicQueue, (char*) &msg, MSG_SIZE, 1);
            _exit(0);
        }

        mq_send(publicQueue, (char*) &msg, MSG_SIZE, 1);
        mq_receive(privateQueue,(char*) &msg, MSG_SIZE, NULL);
        printf("%s", msg.text);
        if (action == 'e') {
            _exit(0);
        }
    }

}