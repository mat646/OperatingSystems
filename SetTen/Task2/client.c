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

char name[100];
int connect_type;
char address[100];
int port;
int socket_desc;

int main(int argc, char **argv) {

    if (argc == 1 || argv[1] == NULL || argv[2] == NULL || argv[3] == NULL || argv[4] == NULL) {
        printf("Parametry wywolania:\n 1. nazwa klienta\n 2. numer portu\n 3. sciezka gniazda UNIX\n 4. polaczenie\n");
        return 0;
    }

    sscanf(argv[1], "%s", name);
    sscanf(argv[2], "%d", &port);
    sscanf(argv[3], "%s", address);
    sscanf(argv[4], "%d", &connect_type);


    //-------------------OPENING SOCKET-------------------

    if (connect_type == 1) {
        socket_desc = socket(AF_INET, SOCK_DGRAM, 0);
        if (socket_desc == -1) {
            printf("inet: can't create socket\n");
            exit(1);
        }

        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons((uint16_t) port);
        if (inet_pton(AF_INET, address, &addr.sin_addr) == 0) {
            printf("inet: can't establish address\n");
            exit(1);
        }

        if (connect(socket_desc, (const struct sockaddr *) &addr, sizeof(addr)) == -1) {
            printf("inet: can't connect\n");
            exit(1);
        }

    } else {
        socket_desc = socket(AF_UNIX, SOCK_DGRAM, 0);
        if (socket_desc == -1) {
            printf("unix: can't create socket\n");
            exit(1);
        }

        struct sockaddr_un addr;
        addr.sun_family = AF_UNIX;
        strcpy(addr.sun_path, address);

        if(bind(socket_desc, (const struct sockaddr *) &addr, sizeof(sa_family_t)) == -1){
            printf("unix: can't connect");
            exit(1);
        }

        if (connect(socket_desc, (const struct sockaddr *) &addr, sizeof(addr)) == -1) {
            printf("unix: can't connect\n");
            exit(1);
        }
    }

    //-------------------LOGGING IN-------------------

    msg log_msg;
    strcpy(log_msg.name, name);
    log_msg.eval1.type = 0;
    if (write(socket_desc, &log_msg, sizeof(log_msg)) < 0) {
        printf("can't log on server\n");
    }


    //-------------------MAIN LOOP-------------------

    while (1) {
        msg *msg1 = malloc(sizeof(msg));
        recv(socket_desc, msg1, sizeof(msg), MSG_WAITALL);

        if (msg1->eval1.type == 0) {
            printf("client with that name already exists\n");
            exit(0);
        } else if (msg1->eval1.type == 2) {
            printf("task received\n");

            int res;
            if (msg1->eval1.operand == '+') {
                res = msg1->eval1.arg1 + msg1->eval1.arg2;
            } else if (msg1->eval1.operand == '-') {
                res = msg1->eval1.arg1 - msg1->eval1.arg2;
            } else if (msg1->eval1.operand == '*') {
                res = msg1->eval1.arg1 * msg1->eval1.arg2;
            } else if (msg1->eval1.operand == '/') {
                res = msg1->eval1.arg1 / msg1->eval1.arg2;
            }

            strcpy(msg1->name, name);

            msg1->eval1.type = 2;
            msg1->eval1.arg1 = res;

            write(socket_desc, msg1, sizeof(msg));

        } else if (msg1->eval1.type == 1) {
            printf("pinged\n");
            strcpy(msg1->name, name);
            write(socket_desc, msg1, sizeof(msg));
        } else if (msg1->eval1.type == 3) {
            printf("Server closed");
            exit(0);
        }

        free(msg1);
    }

}
