//
// Created by Mateusz Sokol on 08.03.18.
//

#include <malloc.h>
#include <stdlib.h>
#include "table_manager.h"

Table *create_table(int num_elem, int block_size) {

    Table *table = malloc(sizeof(Table));
    table->size = num_elem;
    table->block_size = block_size;

    table->values = (char **) calloc((size_t) num_elem, sizeof(char *));
    for (int i = 0; i < num_elem; ++i) {
        table->values[i] = (char *) calloc((size_t) block_size, sizeof(char));
    }

    //filling with data
    for (int i = 0; i < num_elem; ++i) {
        for (int j = 0; j < block_size; ++j) {
            table->values[i][j] = rand_char();
        }
    }
    return table;
};

void delete_table(Table *table) {
    free(table);
};

void add_block(Table *table, int index) {
    table->values[index] = (char *) calloc((size_t) table->block_size, sizeof(char));

    for (int i = 0; i < table->block_size; ++i) {
        table->values[index][i] = rand_char();
    }
};

void delete_block(struct Table *table, int index) {
    free(table->values[index]);
};

char rand_char() {
    return (char) ((rand() % 24) + 97);
};

char *search_table(struct Table *table, int index) {
    int diff = 1000000, closest = 0, sum = 0;
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
