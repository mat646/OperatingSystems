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

int clients[100];
int last_index = 0;
int queue = -1;

void intHandler(int sig) {
    exit(1);
}

void rmQueue(void){
    if(queue > -1){
        if(msgctl(queue, IPC_RMID, NULL) == -1){
            printf("There was some error deleting clients's queue!\n");
        }
        else printf("Client's queue deleted successfully!\n");
    }
}

void compute(msg *msg1) {
    if(msg1 == NULL) return;

    printf("%li\n", msg1->myType);

    switch (msg1->myType) {
        case 1:
            printf("xd");
            break;
        case 2:
            printf("done");
            break;
        default:
            break;
    }
}

int main() {
    atexit(rmQueue);

    signal(SIGINT, intHandler);

    char *home = getenv("HOME");

    key_t key = ftok(home, PROJECT_ID);

    queue = msgget(key, IPC_CREAT | IPC_EXCL | 0666);
    if(queue == -1) printf("Creation of public queue failed!");

    msg msg1;

    while (1) {
        //struct msqid_ds currentState;
        //msgctl(publicID, IPC_STAT, &currentState);

        //if(msgsnd(queue, &msg1, MSG_SIZE, 0) == -1) printf("error\n");
        if(msgrcv(queue, &msg1, MSG_SIZE, 0, 0) < 0) printf("Receiving message failed!");
        //msgrcv(queue, &msg1, MSG_SIZE, 0, 0);
        compute(&msg1);

    }

}

//void login(msg* msg){
//    key_t clientQKey;
//    sscanf(msg->pid, "%d", &clientQKey);
//
//    int clientQID = msgget(clientQKey, 0);
//
//    //int clientPID = msg->senderPID;
//    //msg->msg_type = INIT;
//    //msg->senderPID = getpid();
//
//    if(last_index > 98){
//        printf("Maximum amount of clients reached!\n");
//    }else{
//        clients[last_index] = clientQID;
//        //clientsData[clientCnt++][1] = clientQID;
//        //sprintf(msg->cont, "%d", clientCnt-1);
//        last_index++;
//    }
//
//    msgsnd(clientQID, msg, 100, 0);
//}