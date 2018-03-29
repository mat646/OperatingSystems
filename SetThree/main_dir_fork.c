//
// Created by Mateusz Sokol on 17.03.18.
//

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

void search_dir(char path[100], char sign[10], time_t date);

time_t str_to_time(char date[20]);

void print(char *path, char *time, struct stat status);

int main(int argc, char **argv) {

    if (argc == 1 || argv[1] == NULL || argv[2] == NULL || argv[3] == NULL) {
        printf("Parametry wywolania:\n 1. operacja\n 2. nazwa pliku/plikow\n 3. rozmiar rekordu\n 4. liczba rekordow\n 5. typ funkcji\n");
        return 0;
    }

    char path[100], sign[10], date[20];

    realpath(argv[1], path);
    sscanf(argv[2], "%s", sign);
    sscanf(argv[3], "%s", date);

    search_dir(path, sign, str_to_time(date));

    return 0;
}

void search_dir(char path[100], char sign[10], time_t date) {
    DIR *directory = opendir(path);
    if (directory != NULL) {

        struct dirent *file;
        while ((file = readdir(directory))) {
            if (file->d_name[0] == '.') continue;

            char path1[200];
            strcpy(path1, path);
            strcat(path1, "/");
            strcat(path1, file->d_name);

            struct stat status;
            lstat(path1, &status);

            long timestamp = (status.st_mtim.tv_nsec / 1000000000 + status.st_mtim.tv_sec);

            char buff[20];
            strftime(buff, 20, "%Y-%m-%d", localtime(&timestamp));

            if (sign[0] == '<') {
                if (timestamp < date) {
                    printf("\n%s\n", file->d_name);
                    print(path1, buff, status);
                }
            } else if (sign[0] == '>') {
                if (timestamp > date) {
                    printf("\n%s\n", file->d_name);
                    print(path1, buff, status);
                }
            } else if (sign[0] == '=') {
                if (abs(timestamp - date) < 90000) {
                    printf("\n%s\n", file->d_name);
                    print(path1, buff, status);
                }
            }

            if (!S_ISLNK(status.st_mode) && S_ISDIR(status.st_mode)) {
                pid_t child_pid = vfork();
                if (child_pid==0) {
                    search_dir(path1, sign, date);
                    _exit(0);
                }
            }
        }

        closedir(directory);
    }

}

time_t str_to_time(char date[20]) {
    date[4] = date[7] = '\0';
    struct tm tmdate = {0};
    tmdate.tm_year = atoi(&date[0]) - 1900;
    tmdate.tm_mon = atoi(&date[5]) - 1;
    tmdate.tm_mday = atoi(&date[8]);
    time_t t = mktime(&tmdate);
    return t;
}

void print(char *path, char *time, struct stat status) {
    printf("\nReal path: %s", path);
    printf("\nSize in bytes: %ld", status.st_size);
    printf("\nModify time: %s\n", time);
    printf("File Permissions: ");
    if (S_ISDIR(status.st_mode)) {
        printf("d");
    } else if (S_ISLNK(status.st_mode)) {
        printf("l");
    } else printf("-");
    printf((status.st_mode & S_IRUSR) ? "r" : "-");
    printf((status.st_mode & S_IWUSR) ? "w" : "-");
    printf((status.st_mode & S_IXUSR) ? "x" : "-");
    printf((status.st_mode & S_IRGRP) ? "r" : "-");
    printf((status.st_mode & S_IWGRP) ? "w" : "-");
    printf((status.st_mode & S_IXGRP) ? "x" : "-");
    printf((status.st_mode & S_IROTH) ? "r" : "-");
    printf((status.st_mode & S_IWOTH) ? "w" : "-");
    printf((status.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");

    printf("\n####################\n");
}