//
// Created by Mateusz Sokol on 17.03.18.
//

#ifndef OPERATINGSYSTEMS_FILE_MANAGER_H
#define OPERATINGSYSTEMS_FILE_MANAGER_H

typedef struct file {
    char *file_name;
    int record_size;
    int record_amount;
} file;

void generate(file source);

void sort_sys(file source);

void sort_lib(file source);

void copy_sys(file source, file target);

void copy_lib(file source, file target);


#endif //OPERATINGSYSTEMS_FILE_MANAGER_H
