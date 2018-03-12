//
// Created by Mateusz Sokol on 08.03.18.
//

#ifndef SETONE_TABLE_MANAGER_H
#define SETONE_TABLE_MANAGER_H

typedef struct Table {
    int size;
    int block_size;
    char **values;
} Table;

Table *create_table(int num_elem, int block_size);

void delete_table(Table *table);

void add_block(Table *table, int index);

void delete_block(Table *table, int index);

char *search_table(Table *table, int index);

char rand_char();

#endif //SETONE_TABLE_MANAGER_H
