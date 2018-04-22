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

int publicID = -1;
int privateID = -1;

void sigInt(int sig){
    _exit(0);
}


void rmQueue(void){
    if(privateID > -1){
        if(msgctl(privateID, IPC_RMID, NULL) == -1){
            printf("There was some error deleting clients's queue!\n");
        }
        else printf("Client's queue deleted successfully!\n");
    }
}

int main() {
    atexit(rmQueue);

    signal(SIGINT, sigInt);

    char *home = getenv("HOME");

    key_t key = ftok(home, PROJECT_ID);

    int publicQueue = msgget(key, 0);
    if(publicQueue == -1) printf("Opening queue failed!");

    key_t privateKey = ftok(home, getpid());

    int privateQueue = msgget(privateKey, IPC_CREAT | IPC_EXCL | 0666);


    //register client

    msg msg;
    msg.myType = 1;
    msg.pid = getpid();
    if(msgsnd(publicQueue, &msg, MSG_SIZE, 0) == -1) printf("error\n");
    //msgrcv(privateQueue, &msg, MSG_SIZE, 0, 0);



    while (1) {
        msg.pid = getpid();
        msg.myType = 10;

        msgsnd(publicQueue, &msg, MSG_SIZE, 0);
        //msgrcv(publicQueue, &msg, MSG_SIZE, 0, 0);
        printf("%s", msg.text);


    }

}