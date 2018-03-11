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
    sscanf(argv[3], "%d", &alloc_type); // 1 - dynamic, 0 - static
    sscanf(argv[4], "%p", &ops); // 1 - del&add alter, 2 - del&add serial, 3 - search

    if (alloc_type == 1) {
        printf("\n---Using dynamic allocation---\n");

        void *handle = dlopen("./table_manager.so", RTLD_LAZY);
        if (!handle) { return 120; }
        Table *(*lib_fun)(int num_elem, int block_size);
        lib_fun = (Table *(*)(int num_elem, int block_size)) dlsym(handle, "create_table");
        if (dlerror() != NULL) { return 220; }

        Time time1 = start();

        Table *main_table = (*lib_fun)(num, size);

        end(&time1);

        print(time1, "Creating table:");

        time1 = start();

        for (int i = 0; i < main_table->size; ++i) {
            void (*block_handle)(Table *table, int index);
            block_handle = (void (*)(Table *table, int index)) dlsym(handle, "delete_block");
            if (dlerror() != NULL) { return 221; }
            (*block_handle)(main_table, i);

            block_handle = (void (*)(Table *table, int index)) dlsym(handle, "add_block");
            if (dlerror() != NULL) { return 222; }
            (*block_handle)(main_table, i);
        }

        end(&time1);

        print(time1, "Deleting and adding blocks alternately:");

        time1 = start();

        for (int i = 0; i < main_table->size; ++i) {
            void (*block_handle)(Table *table, int index);
            block_handle = (void (*)(Table *table, int index)) dlsym(handle, "delete_block");
            if (dlerror() != NULL) { return 221; }
            (*block_handle)(main_table, i);
        }
        for (int i = 0; i < main_table->size; ++i) {
            void (*block_handle)(Table *table, int index);
            block_handle = (void (*)(Table *table, int index)) dlsym(handle, "add_block");
            if (dlerror() != NULL) { return 222; }
            (*block_handle)(main_table, i);
        }

        end(&time1);

        print(time1, "Deleting and adding blocks serially:");

        time1 = start();

        char *(*block_handle)(Table *table, int index);
        block_handle = (char *(*)(Table *table, int index)) dlsym(handle, "search_table");
        if (dlerror() != NULL) { return 222; }
        (*block_handle)(main_table, 3);

        char *a = (*block_handle)(main_table, (rand() % main_table->size));

        end(&time1);

        print(time1, "Searching similar block:");

        dlclose(handle);
    } else {
        printf("\n---Using static allocation---\n");

        void *handle = dlopen("./table_manager_static.so", RTLD_LAZY);
        if (!handle) { return 100; }
        Table_static *(*lib_fun)(int num_elem, int block_size);
        lib_fun = (Table_static *(*)(int num_elem, int block_size)) dlsym(handle, "create_table_static");
        if (dlerror() != NULL) { return 200; }

        Time time1 = start();

        Table_static *main_table = (*lib_fun)(num, size);

        end(&time1);

        print(time1, "Creating table:");

        time1 = start();

        void (*block_handle)(Table_static *table, int index);
        block_handle = (void (*)(Table_static *table, int index)) dlsym(handle, "add_block_static");
        if (dlerror() != NULL) { return 200; }

        (*block_handle)(main_table, 2);

        end(&time1);

        print(time1, "Adding blocks:");

        dlclose(handle);

        printf("\n");
        //TODO searching
    }

}