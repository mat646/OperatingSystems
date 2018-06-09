//
// Created by Mateusz Sokol on 05.06.18.
//

#include "contract.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>

#define _GNU_SOURCE

char socket_path[30];
int port_number;
int unixSocketDesc;
int inet;
int epoll;
int clients_fd[100];

pthread_t resolve_thread;
pthread_t ping_thread;

void *server_loop();

void close_all() {

    close(unixSocketDesc);
    close(inet);

}

int main(int argc, char **argv) {

    if (argc == 1 || argv[1] == NULL || argv[2] == NULL) {
        printf("Parametry wywolania:\n 1. numer portu\n 2. sciezka gniazda UNIX\n");
        return 0;
    }

    sscanf(argv[1], "%d", &port_number);
    sscanf(argv[2], "%s", socket_path);


    //-------------------INIT INET----------------------------

    inet = socket(AF_INET, SOCK_STREAM, 0);
    if(inet == -1)
        printf("Was unable to create inet socket\n");

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons((uint16_t) port_number);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int result;
    result = bind(inet, (const struct sockaddr *)&addr, sizeof(addr));
    if(result == -1)
        printf("Was unable to bind inet socket\n");

    result = listen(inet, 10);
    if(result == -1)
        printf("Error occurred, when tired to open inet socket for connection requests\n");


    //-----------------INIT UNIX-------------------------------

    unixSocketDesc = socket(AF_UNIX, SOCK_STREAM, 0);
    if(unixSocketDesc == -1)
        printf("Was unable to create unix socket\n");

    struct sockaddr_un addr1;
    addr1.sun_family = AF_UNIX;
    strcpy(addr1.sun_path, socket_path);

    if (bind(unixSocketDesc, (struct sockaddr *)&(addr1), sizeof(addr1)) != 0){
        perror("failed to bind unix socket");
        exit(1);
    }

    result = listen(unixSocketDesc, 10);
    if (result == -1)
        printf("Error occurred, when tired to open unix socket for connection requests\n");


    //----------------INIT EPOLL------------------------------------


    epoll = epoll_create1(0);
    if (epoll == -1){
        fprintf(stderr, "Failed to create epoll file descriptor\n");
        exit(1);
    }

    struct epoll_event e;
    e.events = EPOLLIN | EPOLLET;
    e.data.fd = unixSocketDesc;
    if (epoll_ctl(epoll,EPOLL_CTL_ADD, unixSocketDesc, &e) == -1){
        fprintf(stderr, "Failed to create epoll file descriptor for LOCAL\n");
        exit(1);
    }

    e.data.fd = inet;
    if (epoll_ctl(epoll,EPOLL_CTL_ADD, inet, &e) == -1){
        fprintf(stderr, "Failed to create epoll file descriptor for Inet\n");
        exit(1);
    }


    pthread_create(&resolve_thread, NULL, server_loop, NULL);

    //pthread_create(&ping_thread, NULL, ping, NULL);


    //---------------------SERVER LOOP----------------


    while (1)
    {

        char op;
        int arg1, arg2;
        scanf("%c %d %d", &op, &arg1, &arg2);

        if (op == 'e') {
            close(epoll);
            close(inet);
            close(unixSocketDesc);
            exit(0);
        }

        msg msg1;
        msg1.eval1.arg1 = arg1;
        msg1.eval1.arg2 = arg2;
        msg1.eval1.type = 2;
        msg1.eval1.operand = 1;

        if(write(clients_fd[0], &msg1, sizeof(msg1)) <= 0){
            printf("d - th client failed to receive message \n");
        }

    }

    return 0;

}


void *server_loop() {

    struct epoll_event events[100];

    while(1) {
        int event = epoll_wait(epoll, events, 100, -1);
        for(int i = 0; i < event; i++){
            if(events[i].data.fd == unixSocketDesc || events[i].data.fd == inet ){

                struct sockaddr new_addr;
                socklen_t new_addr_len = sizeof(new_addr);
                clients_fd[i] =  accept(events[i].data.fd, &new_addr, &new_addr_len);

                struct epoll_event e;
                e.events = EPOLLIN | EPOLLET;
                e.data.fd = clients_fd[i];
                if (epoll_ctl(epoll,EPOLL_CTL_ADD, clients_fd[i], &e) == -1){
                    fprintf(stderr, "Failed to create epoll file descriptor for client\n");
                    fflush(stdout);
                }
                printf("loged user\n");
                fflush(stdout);
            }else{
                //resolve_mess(events[i]);
                msg ms;
                ssize_t bytes = read(events[i].data.fd,&ms ,sizeof(ms));
                printf("message\n");

            }

        }

    }


}