//
// Created by Mateusz Sokol on 10.03.18.
//

#include <stdio.h>
#include <stdlib.h>
#include "table_manager.h"
#include "table_manager_static.h"
#include "time_full.h"

int main(int argc, char **argv) {

    if (argc == 1) {
        printf("Parametry wywolania:\n 1. liczba elementow tablicy\n 2. rozmiar bloku\n 3. sposob alokacji\n 4. operacje\n");
        return 0;
    }

    int num, size, alloc_type;
    char ops[30];
    sscanf(argv[1], "%d", &num);
    sscanf(argv[2], "%d", &size);
    sscanf(argv[3], "%d", &alloc_type); // 1 - dynamic, 0 - static
    sscanf(argv[4], "%s", &ops[0]); // 1 - del&add alter, 2 - del&add serially, 3 - searching

    if (alloc_type == 1) {
        printf("\n---Using dynamic allocation---\n");

        Time time1 = start();

        Table *main_table = create_table(num, size);

        end(&time1);

        print(time1, "Creating table:");

        time1 = start();

        for (int i = 0; i < num; ++i) {
            delete_block(main_table, i);
            add_block(main_table, i);
        }

        end(&time1);

        print(time1, "Deleting and adding blocks alternately:");

        time1 = start();

        for (int i = 0; i < num; ++i) {
            delete_block(main_table, i);
        }
        for (int i = 0; i < num; ++i) {
            add_block(main_table, i);
        }

        end(&time1);

        print(time1, "Deleting and adding blocks serially:");

        time1 = start();

        search_table(main_table, (rand() % main_table->size));

        delete_table(main_table);

        end(&time1);

        print(time1, "Searching similar block:");

    } else {
        printf("\n---Using static allocation---\n");

        Time time1 = start();

        Table_static *main_table = create_table_static(num, size);

        end(&time1);

        print(time1, "Creating table:");

        time1 = start();

        for (int i = 0; i < num; ++i) {
            add_block_static(main_table, i);
        }

        end(&time1);

        print(time1, "Adding blocks:");

        time1 = start();

        search_table_static(main_table, (rand() % main_table->size));

        end(&time1);

        print(time1, "Searching similar block:");

        printf("\n");
    }

    return 0;
}
