//
// Created by Mateusz Sokol on 17.03.18.
//

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

void search_dir(char path[100], char sign[10], time_t date);

time_t str_to_time(char date[20]);

void print(char *path, char *time, struct stat ed);

int main(int argc, char **argv) {

    if (argc == 1) {
        printf("Parametry wywolania:\n 1. sciezka\n");
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

        struct dirent *plik;
        while ((plik = readdir(directory))) {
            if (plik->d_name[0] == '.') continue;

            char path1[200];
            strcpy(path1, path);
            strcat(path1, "/");
            strcat(path1, plik->d_name);

            struct stat ed;
            stat(path1, &ed);

            long timestamp = (ed.st_mtim.tv_nsec / 1000000000 + ed.st_mtim.tv_sec);

            char buff[20];
            strftime(buff, 20, "%Y-%m-%d", localtime(&timestamp));

            if (sign[0] == '<') {
                if (timestamp < date) {
                    print(path1, buff, ed);
                }
            } else if (sign[0] == '>') {
                if (timestamp > date) {
                    print(path1, buff, ed);
                }
            } else if (sign[0] == '=') {
                if (timestamp == date) {
                    print(path1, buff, ed);
                }
            }

            if (S_ISDIR(ed.st_mode)) {
                search_dir(path1, sign, date);
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

void print(char *path, char *time, struct stat ed) {
    printf("\nReal path: %s", path);
    printf("\nSize in bytes: %ld", ed.st_size);
    printf("\nModify time: %s\n", time);
    printf("File Permissions: ");
    printf((S_ISDIR(ed.st_mode)) ? "d" : "-");
    printf((ed.st_mode & S_IRUSR) ? "r" : "-");
    printf((ed.st_mode & S_IWUSR) ? "w" : "-");
    printf((ed.st_mode & S_IXUSR) ? "x" : "-");
    printf((ed.st_mode & S_IRGRP) ? "r" : "-");
    printf((ed.st_mode & S_IWGRP) ? "w" : "-");
    printf((ed.st_mode & S_IXGRP) ? "x" : "-");
    printf((ed.st_mode & S_IROTH) ? "r" : "-");
    printf((ed.st_mode & S_IWOTH) ? "w" : "-");
    printf((ed.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");

    printf("\n####################\n");
}