//
// Created by Mateusz Sokol on 09.03.18.
//

#include <stdlib.h>
#include "table_manager_static.h"

Table_static *create_table_static(int num_elem, int block_size) {

    Table_static *table = malloc(sizeof(Table_static));
    table->size = num_elem;
    table->block_size = block_size;

    char a[num_elem][block_size];
    table->values = (char **) a;
    for (int i = 0; i < block_size; ++i) {
        char b [block_size];
        table->values[i] = b;
    }

    //filling with data
    for (int i = 0; i < num_elem; ++i) {
        for (int j = 0; j < block_size; ++j) {
            table->values[i][j] = 'a';
        }
    }
    return table;
};



void add_block_static(Table_static *table, int index) {
    for (int i = 0; i < table->block_size; ++i) {
        table->values[index][i] = 'b';
    }
};

char *search_table_static(Table_static *table, int index) {
    int diff = 100000, closest = 0, sum = 0;
    for (int i = 0; i < table->block_size; ++i) {
        sum += table->values[index][i];
    }

    for (int i = 0; i < table->size; ++i) {
        if (i == index) continue;

        int recent_sum = 0;
        for (int j = 0; j < table->block_size; ++j) {
            recent_sum += table->values[i][j];
        }

        if (abs(sum - recent_sum) < diff) {
            closest = i;
            diff = abs(sum - recent_sum);
        }
    }

    return table->values[closest];
};