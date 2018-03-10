//
// Created by Mateusz Sokol on 10.03.18.
//

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "table_manager.h"

int main(int argc, char **argv) {

    if (argc == 1) {
        printf("Parametry wywolania:\n 1. liczba elementow tablicy\n 2. rozmiar bloku\n 3. sposob alokacji\n 4. operacje ");
        return 0;
    }

    for (int i = 0; i < 10; ++i) {
        clock_t a = clock();
        printf("%f\n", (float)a);
    }

    int num, size, alloc_type;
    char *ops;
    sscanf(argv[1], "%d", &num);
    sscanf(argv[2], "%d", &size);
    sscanf(argv[3], "%d", &alloc_type);
    sscanf(argv[4], "%p", &ops);

    void *handle = dlopen("./table_manager.so", RTLD_LAZY);
    if(!handle){ return  100; }
    Table *(*lib_fun)(int num_elem, int block_size);
    lib_fun = (Table *(*)(int num_elem, int block_size))dlsym(handle,"create_table");
    if(dlerror() != NULL){ return 200; }
    Table *xd = (*lib_fun)(num, size);
    dlclose(handle);
}