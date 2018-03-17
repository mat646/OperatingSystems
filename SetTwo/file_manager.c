//
// Created by Mateusz Sokol on 17.03.18.
//

#include "file_manager.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

void generate(file source) {
    char block[source.record_size];
    int handle = open(source.file_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    for (int i = 0; i < source.record_amount; ++i) {
        for (int j = 0; j < source.record_size - 1; ++j) {
            block[j] = (char) ((rand() % 23) + 97);
        }
        block[source.record_size - 1] = '\n';
        write(handle, block, source.record_size);
    }

    close(handle);
};

void sort_sys(file source) {
    char block1[source.record_size];
    char block2[source.record_size];
    int handle = open(source.file_name, O_RDWR, S_IRUSR | S_IWUSR);
    for (int j = 0; j < source.record_amount; ++j) {
        for (int i = 0; i < source.record_amount-1; ++i) {
            lseek(handle, i * source.record_size, SEEK_SET);
            read(handle, block1, sizeof(block1));
            lseek(handle, (i+1) * source.record_size, SEEK_SET);
            read(handle, block2, sizeof(block2));
            if (block1[0] > block2[0]) {
                lseek(handle, i * source.record_size, SEEK_SET);
                write(handle, block2, sizeof(block2));
                lseek(handle, (i+1) * source.record_size, SEEK_SET);
                write(handle, block1, sizeof(block1));
            }
        }
    }

    close(handle);
};

void sort_lib(file source) {
    char block1[source.record_size];
    char block2[source.record_size];
    FILE *plik = fopen(source.file_name, "r+");
    if (plik) {
        for (int i = 0; i < source.record_amount; ++i) {
            for (int j = 0; j < source.record_amount-1; ++j) {
                fseek(plik, j * source.record_size, 0);
                fread(block1, sizeof(char), source.record_size, plik);
                fseek(plik, (j+1) * source.record_size, 0);
                fread(block2, sizeof(char), source.record_size, plik);
                if (block1[0] > block2[0]) {
                    fseek(plik, j * source.record_size, 0);
                    fwrite(block2, sizeof(char), source.record_size, plik);
                    fseek(plik, (j+1) * source.record_size, 0);
                    fwrite(block1, sizeof(char), source.record_size, plik);
                }
            }
        }
    }

    fclose(plik);
};

void copy_sys(file source, file target) {
    char block[source.record_size];
    int length;
    int handle_source = open(source.file_name, O_RDONLY);
    int handle_target = open(target.file_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    while ((length = read(handle_source, block, sizeof(block))) > 0)
        write(handle_target, block, length);

    close(handle_source);
    close(handle_target);
};

void copy_lib(file source, file target) {
    char napis[source.record_size];
    FILE *plik = fopen(source.file_name, "r");
    FILE *plik2 = fopen(target.file_name, "w");
    if (plik && plik2) {
        for (int i = 0; i < source.record_amount; ++i) {
            fread(napis, sizeof(char), source.record_size, plik);
            fwrite(napis, sizeof(char), source.record_size, plik2);
        }

        fclose(plik);
        fclose(plik2);
    }
};