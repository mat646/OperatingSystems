//
// Created by Mateusz Sokol on 10.03.18.
//

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "table_manager.h"
#include "time_full.h"
#include "table_manager_static.h"

int main(int argc, char **argv) {

    if (argc == 1) {
        printf("Parametry wywolania:\n 1. liczba elementow tablicy\n 2. rozmiar bloku\n 3. sposob alokacji\n 4. operacje ");
        return 0;
    }

    int num, size, alloc_type;
    char *ops;
    sscanf(argv[1], "%d", &num);
    sscanf(argv[2], "%d", &size);
    sscanf(argv[3], "%d", &alloc_type);
    sscanf(argv[4], "%p", &ops);

    if (alloc_type == 1) {
        printf("---Using dynamic allocation---\n");

        void *handle = dlopen("./table_manager.so", RTLD_LAZY);
        if(!handle){ return  120; }
        Table *(*lib_fun)(int num_elem, int block_size);
        lib_fun = (Table *(*)(int num_elem, int block_size))dlsym(handle,"create_table");
        if(dlerror() != NULL){ return 220; }


        Time time1 = start();

        Table *xd = (*lib_fun)(num, size);

        end(&time1);

        printf("Creating table:\n");
        printf("user time: %f\n", time1.user_time);
        printf("system time: %f\n", time1.system_time);
        printf("real time: %f\n", time1.real_time);

        dlclose(handle);
    } else {
        printf("---Using static allocation---\n");

        void *handle = dlopen("./table_manager_static.so", RTLD_LAZY);
        if(!handle){ return  100; }
        Table_static *(*lib_fun)(int num_elem, int block_size);
        lib_fun = (Table_static *(*)(int num_elem, int block_size))dlsym(handle,"create_table_static");
        if(dlerror() != NULL){ return 200; }


        Time time1 = start();

        Table_static *xd = (*lib_fun)(num, size);

        end(&time1);

        printf("Creating table:\n");
        printf("user time: %f\n", time1.user_time);
        printf("system time: %f\n", time1.system_time);
        printf("real time: %f\n", time1.real_time);

        dlclose(handle);
    }

}