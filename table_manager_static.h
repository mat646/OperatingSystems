//
// Created by Mateusz Sokol on 09.03.18.
//

#ifndef SETONE_TABLE_MANAGER_STATIC_H
#define SETONE_TABLE_MANAGER_STATIC_H

typedef struct Table_static {
    int size;
    int block_size;
    char **values;
} Table_static;

Table_static *create_table_static(int num_elem, int block_size);

void add_block_static(Table_static *table, int index);

char *search_table_static(Table_static *table, int index);

#endif //SETONE_TABLE_MANAGER_STATIC_H
