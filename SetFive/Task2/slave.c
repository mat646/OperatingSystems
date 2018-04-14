//
// Created by Mateusz Sokol on 14.04.18.
//

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
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

    int pid = getpid();

    int fd;
    char * myfifo = "/tmp/myfifo";
    fd = open(myfifo, O_WRONLY);

    printf("%d\n", pid);

    for (int i = 0; i < N; ++i) {

        write(fd, &pid, sizeof(int));

        FILE *fp;
        char path1[MAX_BUF];

        fp = popen("date", "r");
        if (fp == NULL)
            printf("An error occurred.");

        while (fgets(path1, MAX_BUF, fp) != NULL)
            write(fd, path1, sizeof(path1));

        pclose(fp);

        sleep(rand() % 5);
    }

    close(fd);

    return 0;
}