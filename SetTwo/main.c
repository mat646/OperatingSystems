//
// Created by Mateusz Sokol on 16.03.18.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "file_manager.h"

int main(int argc, char **argv) {

    srand ( time(NULL) );

    if (argc == 1) {
        printf("Parametry wywolania:\n 1. nazwa pliku\n 2. rozmiar rekordu\n 3. liczba rekordow\n 4. typ funkcji\n");
        return 0;
    }

    int record_size, record_amount;
    char name[20];
    sscanf(argv[3], "%d", &record_amount);
    sscanf(argv[2], "%d", &record_size);
    sscanf(argv[1], "%s", name);

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

    //TODO time measurement

    generate(file1);

    copy_sys(file1, file2);

    sort_sys(file2);

    copy_lib(file1, file3);

    sort_lib(file3);

}