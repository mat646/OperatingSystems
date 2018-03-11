//
// Created by Mateusz Sokol on 10.03.18.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include "table_manager.h"
#include "table_manager_static.h"
#include "time_full.h"

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

        Time time1 = start();

        Table *xd = create_table(num, size);

        end(&time1);

        printf("Creating table:\n");
        printf("user time: %f\n", time1.user_time);
        printf("system time: %f\n", time1.system_time);
        printf("real time: %f\n", time1.real_time);

        time1 = start();

        for (int i = 0; i < num; ++i) {
            delete_block(xd,i);
            add_block(xd, i);
        }

        end(&time1);

        printf("Deleting and adding blocks alternately:\n");
        printf("user time: %f\n", time1.user_time);
        printf("system time: %f\n", time1.system_time);
        printf("real time: %f\n", time1.real_time);

        time1 = start();

        for (int i = 0; i < num; ++i) {
            delete_block(xd,i);
        }
        for (int i = 0; i < num; ++i) {
            add_block(xd, i);
        }

        end(&time1);

        printf("Deleting and adding blocks serially:\n");
        printf("user time: %f\n", time1.user_time);
        printf("system time: %f\n", time1.system_time);
        printf("real time: %f\n", time1.real_time);

        time1 = start();

        char *a = search_table(xd, (rand()%xd->size));

        delete_table(xd);

        end(&time1);

        printf("Searching similar block:\n");
        printf("user time: %f\n", time1.user_time);
        printf("system time: %f\n", time1.system_time);
        printf("real time: %f\n", time1.real_time);

    } else {
        printf("---Using static allocation---\n");

        Time time1 = start();

        Table_static *xd = create_table_static(num, size);

        end(&time1);

        printf("Creating table:\n");
        printf("user time: %f\n", time1.user_time);
        printf("system time: %f\n", time1.system_time);
        printf("real time: %f\n", time1.real_time);

        time1 = start();

        for (int i = 0; i < num; ++i) {
            add_block_static(xd, i);
        }

        end(&time1);

        printf("Adding blocks:\n");
        printf("user time: %f\n", time1.user_time);
        printf("system time: %f\n", time1.system_time);
        printf("real time: %f\n", time1.real_time);

        time1 = start();

        char *a = search_table_static(xd, (rand()%xd->size));

        end(&time1);

        printf("Searching similar block:\n");
        printf("user time: %f\n", time1.user_time);
        printf("system time: %f\n", time1.system_time);
        printf("real time: %f\n", time1.real_time);
    }

    return 0;
}
