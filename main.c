#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/times.h>
#include <time.h>
#include "table_manager.h"
#include "table_manager_static.h"

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


    /****alloc***/

    if (alloc_type == 1) {
        Table *xd = create_table(num, size);

        delete_block(xd,2);

        add_block(xd, 2);

        char *a = search_table(xd, 2);

        printf("%c\n", a[0]);

        delete_table(xd);
    } else {
        Table_static *xd = create_table_static(num, size);

        //add_block_static(xd, 2);

    }

    /****alloc***/

    //printf("%d\n%d",sizeof(char), sizeof(char*));

    printf("Hello, World!\n");
    return 0;
}