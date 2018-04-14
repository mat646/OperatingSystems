//
// Created by Mateusz Sokol on 14.04.18.
//

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <printf.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUF 1024

int main(int argc, char **argv)
{
    if (argc == 1 || argv[1] == NULL || argv[2] == NULL) {
        printf("Parametry wywolania:\n 1. sciezka\n 2. liczba N\n");
        return 0;
    }

    int N;
    char path[60];
    sscanf(argv[1], "%s", path);
    sscanf(argv[2], "%d", &N);

    int fd;
    char * myfifo = "/tmp/myfifo";

    mkfifo(myfifo, 0666);

    char buf[MAX_BUF];
    int a;

    fd = open(myfifo, O_RDONLY);

    for (int i = 0; i < N; ++i) {
        read(fd, &a, sizeof(int));
        printf("%d\n", a);

        read(fd, buf, MAX_BUF);
        printf("%s\n", buf);
    }

    close(fd);
    unlink(myfifo);

    return 0;
}