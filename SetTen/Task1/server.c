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

char socket_path[30];
int port_number;
int unix_socket;
int inet_socket;
int epoll;
int clients_fd[100];
char clients_names[100][100];
int clients_ping[100];
int last_index = 0;
pthread_t resolve_thread;
pthread_t ping_thread;

void *server_loop();

void *ping_loop();

void close_all() {
    close(epoll);
    close(unix_socket);
    close(inet_socket);
}

int main(int argc, char **argv) {

    if (argc == 1 || argv[1] == NULL || argv[2] == NULL) {
        printf("Parametry wywolania:\n 1. numer portu\n 2. sciezka gniazda UNIX\n");
        return 0;
    }

    sscanf(argv[1], "%d", &port_number);
    sscanf(argv[2], "%s", socket_path);

    //-------------------INIT INET-------------------

    inet_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (inet_socket == -1) {
        printf("inet: can't create socket\n");
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons((uint16_t) port_number);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int result = bind(inet_socket, (const struct sockaddr *) &addr, sizeof(addr));
    if (result == -1) {
        printf("inet: can't bind socket\n");
        exit(1);
    }

    result = listen(inet_socket, 10);
    if (result == -1) {
        printf("inet: can't open socket for connection\n");
        exit(1);
    }


    //-------------------INIT UNIX-------------------

    unix_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (unix_socket == -1) {
        printf("unix: can't create socket\n");
        exit(1);
    }

    struct sockaddr_un addr1;
    addr1.sun_family = AF_UNIX;
    strcpy(addr1.sun_path, socket_path);
    result = bind(unix_socket, (struct sockaddr *) &(addr1), sizeof(addr1));
    if (result != 0) {
        printf("unix: can't bind socket\n");
        exit(1);
    }

    result = listen(unix_socket, 10);
    if (result == -1) {
        printf("unix: can't open socket for connection\n");
        exit(1);
    }


    //-------------------INIT EPOLL-------------------

    epoll = epoll_create1(0);
    if (epoll == -1) {
        printf("Can't create epoll file\n");
        exit(1);
    }

    struct epoll_event e;
    e.events = EPOLLIN | EPOLLET;
    e.data.fd = unix_socket;
    if (epoll_ctl(epoll, EPOLL_CTL_ADD, unix_socket, &e) == -1) {
        fprintf(stderr, "unix: Can't create epoll file\n");
        exit(1);
    }

    e.data.fd = inet_socket;
    if (epoll_ctl(epoll, EPOLL_CTL_ADD, inet_socket, &e) == -1) {
        fprintf(stderr, "inet: Can't create epoll file\n");
        exit(1);
    }


    //-------------------CREATE THREADS-------------------

    pthread_create(&resolve_thread, NULL, server_loop, NULL);

    pthread_create(&ping_thread, NULL, ping_loop, NULL);

    for (int i = 0; i < 100; ++i) {
        clients_ping[i] = 1000;
    }


    //-------------------INPUT LOOP-------------------

    while (1) {
        char op;
        int arg1, arg2;
        scanf(" %c %d %d", &op, &arg1, &arg2);

        if (op == 'e') {
            close_all();
            exit(0);
        }

        msg msg1;
        msg1.eval1.arg1 = arg1;
        msg1.eval1.arg2 = arg2;
        msg1.eval1.type = 2;
        msg1.eval1.operand = op;

        for (int i = 0; i < 100; ++i) {
            if (clients_ping[i] < 10) {
                if (write(clients_fd[i], &msg1, sizeof(msg1)) <= 0) {
                    printf("Can't send message\n");
                }
            }
        }
    }

}

void *server_loop() {

    struct epoll_event events[100];

    while (1) {
        int event = epoll_wait(epoll, events, 100, -1);
        for (int i = 0; i < event; i++) {
            if (events[i].data.fd == unix_socket || events[i].data.fd == inet_socket) {

                struct sockaddr new_addr;
                socklen_t new_addr_len = sizeof(new_addr);
                clients_fd[last_index] = accept(events[i].data.fd, &new_addr, &new_addr_len);
                clients_ping[last_index] = 0;

                struct epoll_event e;
                e.events = EPOLLIN | EPOLLET;
                e.data.fd = clients_fd[last_index];
                if (epoll_ctl(epoll, EPOLL_CTL_ADD, clients_fd[last_index], &e) == -1) {
                    printf("Can't create epoll file\n");
                }

            } else {
                msg ms;
                read(events[i].data.fd, &ms, sizeof(ms));

                if (ms.eval1.type == 1) {
                    printf("Ping received\n");
                    for (int j = 0; j < last_index; ++j) {
                        if(strcmp(ms.name, clients_names[j]) == 0) {
                            clients_ping[j] = 0;
                        }
                    }

                } else if (ms.eval1.type == 2) {
                    printf("Result from %s is: %d\n", ms.name, ms.eval1.arg1);
                } else if (ms.eval1.type == 0) {
                    printf("Logging now\n");
                    int stat = 0;
                    for (int j = 0; j < last_index; ++j) {
                        if (strcmp(ms.name, clients_names[j]) == 0) {
                            close(events[i].data.fd);
                            clients_ping[last_index] = 100;
                            if (write(clients_fd[last_index], &ms, sizeof(ms)) <= 0) {
                                printf("Can't send message\n");
                            }
                            stat = 1;
                        }
                    }
                    if (stat == 0) {
                        strcpy(clients_names[last_index], ms.name);
                        last_index++;
                    }
                }

            }

        }

    }

}

void *ping_loop() {

    while(1) {
        sleep(3);
        for (int i = 0; i < last_index; ++i) {
            if (clients_ping[i] < 10) {
                clients_ping[i]++;
                msg msg1;
                msg1.eval1.type = 1;
                if (send(clients_fd[i], &msg1, sizeof(msg1), MSG_NOSIGNAL) <= 0) {
                    printf("Can't send message\n");
                }
            }
        }
    }
}