//
// Created by Mateusz Sokol on 16.03.18.
//

#include <stdio.h>
#include <stdlib.h>
#include "file_manager.h"

int main(int argc, char **argv) {

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

    generate(file1);

    //copy_lib("we", "wy", record_size, record_amount);

    sort_sys(file1);

}