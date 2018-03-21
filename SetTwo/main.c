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

    if (argc == 1 || argv[1] == NULL || argv[2] == NULL || argv[3] == NULL || argv[4] == NULL || argv[5] == NULL) {
        printf("Parametry wywolania:\n 1. operacja\n 2. nazwa pliku/plikow\n 3. rozmiar rekordu\n 4. liczba rekordow\n 5. typ funkcji\n");
        return 0;
    }

    int record_size, record_amount;
    char name[20], name_copy[20], action[10], type[5], label[30];
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
        if(action[0] != 'g')
            sscanf(argv[5], "%s", type);
    }

    file file1;
    file1.file_name = name;
    file1.record_size = record_size;
    file1.record_amount = record_amount;

    file file2;
    file2.file_name = name_copy;
    file2.record_size = record_size;
    file2.record_amount = record_amount;

    Time time1 = start();

    if (action[0] == 'c') {
        if (type[0] == 's') {
            copy_sys(file1, file2);
            sscanf("copying with sys:", "%s", label);
        } else {
            copy_lib(file1, file2);
            sscanf("copying with lib:", "%s", label);
        }
    } else if(action[0] == 'g') {
        generate(file1);
        sscanf("creating file:", "%s", label);
    } else if(action[0] == 's') {
        if (type[0] == 's') {
            sort_sys(file1);
            sscanf("sorting with sys:", "%s", label);
        } else {
            sort_lib(file1);
            sscanf("sorting with lib:", "%s", label);
        }
    }

    end(&time1);

    print(time1, label);
}