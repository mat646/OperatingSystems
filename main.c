#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include "table_manager.h"
#include "table_manager_static.h"
#include "time_full.h"

int main(int argc, char **argv) {

    //srand(time(NULL));

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

    if (alloc_type == 1) { //dynamic

        Time time1 = start();

        Table *xd = create_table(num, size);

        end(&time1);

        printf("%f\n", time1.user_time);

        time1 = start();

        for (int i = 0; i < num; ++i) {
            delete_block(xd,i);
            add_block(xd, i);
        }

        end(&time1);

        printf("%f\n", time1.user_time);

        time1 = start();

        char *a = search_table(xd, 2);

        printf("%s\n", a);

        delete_table(xd);

        end(&time1);

        printf("%f\n", time1.user_time);

    } else { //static

        Table_static *xd = create_table_static(num, size);
        
        printf("%c", xd->values[0][0]);
        printf("%c", xd->values[0][1]);
        printf("%c", xd->values[0][2]);
        printf("%c", xd->values[0][3]);
        printf("\n");

        add_block_static(xd, 2);

    }

    return 0;
}
