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
int soc;

int main(int argc, char **argv) {

    if (argc == 1 || argv[1] == NULL || argv[2] == NULL || argv[3] == NULL || argv[4] == NULL) {
        printf("Parametry wywolania:\n 1. nazwa klienta\n 2. numer portu\n 3. sciezka gniazda UNIX\n 4. polaczenie\n");
        return 0;
    }

    sscanf(argv[1], "%s", name);
    sscanf(argv[2], "%d", &port);
    sscanf(argv[3], "%s", address);
    sscanf(argv[4], "%d", &connect_type);


    //-----OPENING SOCKET---------

    if (connect_type == 1) {
        soc = socket(AF_INET,SOCK_STREAM,0);
        if(soc == -1){
            printf("failed to create socket");
            exit(1);

        }
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        if(inet_pton(AF_INET, address, &addr.sin_addr) == 0){
            perror("failed to establish address");
            exit(1);
        }
        addr.sin_port = htons((uint16_t) port);

        if(connect(soc, (const struct sockaddr *) &addr,  sizeof(addr)) == -1){
            perror("failed to establish connection");
            exit(1);
        }
    } else {
        soc = socket(AF_UNIX, SOCK_STREAM, 0);
        if(soc == -1){
            printf("failed to create socket");
            exit(1);
        }
        struct sockaddr_un addr;
        addr.sun_family = AF_UNIX;
        strcpy(addr.sun_path, address);

        if(connect(soc, (const struct sockaddr *) &addr,  sizeof(addr)) == -1){
            perror("failed to establish connection");
            exit(1);
        }
    }

    //--------LOGGING IN----------------------

    msg ms;
    strcpy(ms.name, name);
    ms.eval1.type = 0;
    int z;
    if((z = write(soc, &ms, sizeof(ms))) < 0){
        perror("failed to log to");
    }


    //----------LOOP------------------

    while (1) {
        msg msg1;
        ssize_t bytes = recv(soc, &msg1, sizeof(msg1), MSG_WAITALL);

        if (msg1.eval1.type == 0) {
            printf("there is already client with that name!");
        } else if(msg1.eval1.type == 2) {
            printf("task recived\n");
            fflush(stdout);

            int res;

            if (msg1.eval1.operand == 0) {
                res = msg1.eval1.arg1 + msg1.eval1.arg2;
            } else if (msg1.eval1.operand == 1) {
                res = msg1.eval1.arg1 - msg1.eval1.arg2;
            } else if (msg1.eval1.operand == 2) {
                res = msg1.eval1.arg1 * msg1.eval1.arg2;
            } else if (msg1.eval1.operand == 3) {
                res = msg1.eval1.arg1 / msg1.eval1.arg2;
            }

            strcpy(msg1.name, name);

            msg1.eval1.type = 4;
            msg1.eval1.arg1 = res;

            write(soc, &msg1, sizeof(msg1));

        } else if(msg1.eval1.type == 1) {
            printf("Pinged!\n");
            fflush(stdout);
            write(soc, &msg1, sizeof(msg1));
        }


    }


}
