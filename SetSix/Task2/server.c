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

int clients_queue[100];
int clients_pid[100];
int last_index = 0;
int main_queue = -1;

void sig_int(int sig) {
    _exit(0);
}

void delete_queue(void){
    if(main_queue > -1){
        mq_close(main_queue);
    }
}

void login_action(msg *msg);
void mirror_action(msg *msg);
void calc_action(msg *msg);
void time_action(msg *msg);
void end_action(msg *msg);

void compute(msg *msg1) {
    if(msg1 == NULL) return;
    switch (msg1->type) {
        case MIRROR:
            mirror_action(msg1);
            break;
        case CALC:
            calc_action(msg1);
            break;
        case TIME:
            time_action(msg1);
            break;
        case LOGIN:
            login_action(msg1);
            break;
        case END:
            end_action(msg1);
            break;
        default:
            break;
    }
}

int main() {
    atexit(delete_queue);

    signal(SIGINT, sig_int);

    struct mq_attr posixAttr;
    posixAttr.mq_maxmsg = 9;
    posixAttr.mq_msgsize = MSG_SIZE;

    main_queue = mq_open("/server", O_RDONLY | O_CREAT | O_EXCL, 0666, &posixAttr);

    msg msg1;

    while (1) {
        if(mq_receive(main_queue,(char*) &msg1, MSG_SIZE, NULL) < 0) printf("Receiving message failed!");
        compute(&msg1);
    }

}

int get_client_queue(pid_t pid) {
    for(int i=0; i<100; i++){
        if(clients_pid[i] == pid) return clients_queue[i];
    }
    return -1;
}

void login_action(msg *msg){
    int clientQID = mq_open("/client", O_WRONLY);

    if(last_index > 98){
        printf("Full clients\n");
    }else{
        clients_queue[last_index] = clientQID;
        clients_pid[last_index] = msg->pid;
        printf("Registered %d\n", clients_pid[last_index]);
        last_index++;
    }
}

void mirror_action(msg *msg){
    printf("Got %d\n", msg->pid);

    int clientQueueID = get_client_queue(msg->pid);

    int msgLen = (int) strlen(msg->text);
    if(msg->text[msgLen-1] == '\n') msgLen--;

    for(int i=0; i < msgLen / 2; i++) {
        char buff = msg->text[i];
        msg->text[i] = msg->text[msgLen - i - 1];
        msg->text[msgLen - i - 1] = buff;
    }

    mq_send(clientQueueID, (char*) msg, MSG_SIZE, 1);
}

void calc_action(msg *msg) {
    printf("Got %d\n", msg->pid);

    int clientQueueID = get_client_queue(msg->pid);

    char *token;
    char *first;
    char *second;

    /* get the first token */
    token = strtok(msg->text, " ");
    first = strtok(NULL, " ");
    second = strtok(NULL, " ");

    char *end;
    int f = strtol(first, &end, 10);
    int s = strtol(second, &end, 10);

    if (strcmp(token,"ADD") == 0) {
        int someInt = f + s;
        char str[12];
        sprintf(str, "%d", someInt);
        strcpy(msg->text, str);
        mq_send(clientQueueID, (char*) msg, MSG_SIZE, 1);
    } else if (strcmp(token,"SUB") == 0) {
        int someInt = f - s;
        char str[12];
        sprintf(str, "%d", someInt);
        strcpy(msg->text, str);
        mq_send(clientQueueID, (char*) msg, MSG_SIZE, 1);
    } else if (strcmp(token,"MUL") == 0) {
        int someInt = f * s;
        char str[12];
        sprintf(str, "%d", someInt);
        strcpy(msg->text, str);
        mq_send(clientQueueID, (char*) msg, MSG_SIZE, 1);
    }
}

char* convertTime(const time_t* mtime){
    char* buff = malloc(sizeof(char) * 30);
    struct tm * timeinfo;
    timeinfo = localtime (mtime);
    strftime(buff, 20, "%b %d %H:%M", timeinfo);
    return buff;
}

void time_action(msg *msg) {
    printf("Got %d\n", msg->pid);

    time_t timestamp;
    time(&timestamp);
    char* timeStr = convertTime(&timestamp);

    sprintf(msg->text, "%s", timeStr);
    free(timeStr);

    int clientQueueID = get_client_queue(msg->pid);

    mq_send(clientQueueID, (char*) msg, MSG_SIZE, 1);
}

void end_action(msg *msg) {
    mq_close(main_queue);
    _exit(0);
}
