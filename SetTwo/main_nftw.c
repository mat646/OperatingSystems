//
// Created by Mateusz Sokol on 17.03.18.
//

#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <ftw.h>

time_t date;
char sign[1];

time_t str_to_time(char date[20]) {
    date[4] = date[7] = '\0';
    struct tm tmdate = {0};
    tmdate.tm_year = atoi(&date[0]) - 1900;
    tmdate.tm_mon = atoi(&date[5]) - 1;
    tmdate.tm_mday = atoi(&date[8]);
    time_t t = mktime(&tmdate);
    return t;
}

int print(const char *path, const struct stat *status, int tflag, struct FTW *ftwbuf) {

    long timestamp = status->st_mtime;

    if (sign[0] == '<') {
        if (timestamp < date) {
            return 0;
        }
    } else if (sign[0] == '>') {
        if (timestamp > date) {
            return 0;
        }
    } else if (sign[0] == '=') {
        if (abs(timestamp - date) > 70000) {
            return 0;
        }
    }

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
        printf("Parametry wywolania:\n 1. sciezka\n 2. Symbol\n 3. Data\n");
        return 0;
    }

    char path[100], arg_date[20];

    realpath(argv[1], path);
    sscanf(argv[2], "%s", sign);
    sscanf(argv[3], "%s", arg_date);

    date = str_to_time(arg_date);

    nftw(path, print, 50, FTW_DEPTH|FTW_PHYS);
}