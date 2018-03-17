//
// Created by Mateusz Sokol on 16.03.18.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "file_manager.h"
#include "time_full.h"

int main(int argc, char **argv) {

    srand ( time(NULL) );

    if (argc == 1) {
        printf("Parametry wywolania:\n 1. nazwa pliku\n 2. rozmiar rekordu\n 3. liczba rekordow\n 4. typ funkcji\n");
        return 0;
    }

    int record_size, record_amount;
    char name[20], name_copy[20], action[10], type[5];
    sscanf(argv[1], "%s", action);
    if(action[0] == 'c') {
        sscanf(argv[2], "%s", name);
        sscanf(argv[3], "%s", name_copy);
        sscanf(argv[4], "%d", &record_size);
        sscanf(argv[5], "%d", &record_amount);
        sscanf(argv[6], "%s", type);
    } else {
        sscanf(argv[2], "%s", name);
        sscanf(argv[3], "%d", &record_size);
        sscanf(argv[4], "%d", &record_amount);
        sscanf(argv[5], "%s", type);
    }



    file file1;
    file1.file_name = name;
    file1.record_size = record_size;
    file1.record_amount = record_amount;

    char name2[20] = "test2";

    file file2;
    file2.file_name = name2;
    file2.record_size = record_size;
    file2.record_amount = record_amount;

    char name3[20] = "test3";

    file file3;
    file3.file_name = name3;
    file3.record_size = record_size;
    file3.record_amount = record_amount;


    Time time1 = start();

    generate(file1);

    end(&time1);

    print(time1, "Creating file:");


    time1 = start();

    copy_sys(file1, file2);

    end(&time1);

    print(time1, "copying with sys:");


    time1 = start();

    sort_sys(file2);

    end(&time1);

    print(time1, "sorting with sys:");


    time1 = start();

    copy_lib(file1, file3);

    end(&time1);

    print(time1, "copying with lib:");


    time1 = start();

    sort_lib(file3);

    end(&time1);

    print(time1, "sorting with lib:");
}