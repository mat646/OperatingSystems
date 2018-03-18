//
// Created by Mateusz Sokol on 17.03.18.
//

#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <ftw.h>

int print(const char *path, const struct stat *status, int tflag, struct FTW *ftwbuf) {

    char timeBuffer[30];
    strftime(timeBuffer, 20, "%Y-%m-%d %H:%M:%S", localtime(&(status->st_mtime)));

    printf("\nReal path: %s", path);
    printf("\nSize in bytes: %ld", status->st_size);
    printf("\nModify time: %s\n", timeBuffer);
    printf("File Permissions: ");
    printf((S_ISDIR(status->st_mode)) ? "d" : "-");
    printf((status->st_mode & S_IRUSR) ? "r" : "-");
    printf((status->st_mode & S_IWUSR) ? "w" : "-");
    printf((status->st_mode & S_IXUSR) ? "x" : "-");
    printf((status->st_mode & S_IRGRP) ? "r" : "-");
    printf((status->st_mode & S_IWGRP) ? "w" : "-");
    printf((status->st_mode & S_IXGRP) ? "x" : "-");
    printf((status->st_mode & S_IROTH) ? "r" : "-");
    printf((status->st_mode & S_IWOTH) ? "w" : "-");
    printf((status->st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");

    printf("\n####################\n");
    return 0;
}

int main(int argc, char **argv) {

    if (argc == 1) {
        printf("Parametry wywolania:\n 1. sciezka\n");
        return 0;
    }

    char path[100], sign[10], date[20];

    realpath(argv[1], path);
    sscanf(argv[2], "%s", sign);
    sscanf(argv[3], "%s", date);

    nftw(path, print, 50, FTW_DEPTH|FTW_PHYS);
}