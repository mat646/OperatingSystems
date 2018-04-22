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

int clients_queue[100];
int clients_pid[100];
int last_index = 0;
int queue = -1;

void sig_int(int sig) {
    _exit(1);
}

void rmQueue(void){
    if(queue > -1){
        msgctl(queue, IPC_RMID, NULL);
    }
}

void doLogin(msg* msg);
void doMirror(msg* msg);
void doCalc(msg* msg);
void doTime(msg* msg);
void doEnd(msg* msg);

void compute(msg *msg1) {
    if(msg1 == NULL) return;
    switch (msg1->type) {
        case 1:
            //printf("mirror");
            doMirror(msg1);
            break;
        case 2:
            printf("calc");
            //doCalc(msg1);
            break;
        case 3:
            printf("time");
            //doTime(msg1);
            break;
        case 4:
            //printf("login");
            doLogin(msg1);
            break;
        case 5:
            printf("end");
            //doEnd(msg1);
            break;
        default:
            break;
    }
}

int main() {
    atexit(rmQueue);

    signal(SIGINT, sig_int);

    char *home = getenv("HOME");

    key_t key = ftok(home, QUEUE_ID);

    queue = msgget(key, IPC_CREAT | IPC_EXCL | 0666);

    msg msg1;

    while (1) {
        //struct msqid_ds currentState;
        //msgctl(publicID, IPC_STAT, &currentState);

        if(msgrcv(queue, &msg1, MSG_SIZE, 0, 0) < 0) printf("Receiving message failed!");
        compute(&msg1);
    }

}

void doLogin(msg* msg){
    key_t clientQKey;

    int clientQID = msgget(msg->key, 0);

    printf("clientsqueue %d\n", clientQID);

    //int clientPID = msg->senderPID;
    //msg->msg_type = INIT;
    //msg->senderPID = getpid();

    if(last_index > 98){
        printf("Maximum amount of clients_queue reached!\n");
    }else{
        clients_queue[last_index] = clientQID;
        clients_pid[last_index] = msg->pid;
        //clientsData[clientCnt++][1] = clientQID;
        //sprintf(msg->cont, "%d", clientCnt-1);

        printf("Registered %d\n", clients_pid[last_index]);
        last_index++;
    }

    //msgsnd(clientQID, msg, 100, 0);
}

void doMirror(msg* msg){

    printf("Got %d\n", msg->pid);

    int clientQueueID = -1;
    for(int i=0; i<100; i++){
        if(clients_pid[i] == msg->pid) clientQueueID = clients_queue[i];
    }
    if(clientQueueID == -1) return;

    printf("Got message\n");

    int msgLen = (int) strlen(msg->text);
    if(msg->text[msgLen-1] == '\n') msgLen--;

    for(int i=0; i < msgLen / 2; i++) {
        char buff = msg->text[i];
        msg->text[i] = msg->text[msgLen - i - 1];
        msg->text[msgLen - i - 1] = buff;
    }

    if(msgsnd(clientQueueID, msg, MSG_SIZE, 0) == -1) printf("MIRROR response failed!");
}