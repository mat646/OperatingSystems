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
    int handle = open(source.file_name,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);

    for (int i = 0; i < source.record_amount; ++i) {
        for (int j = 0; j < source.record_size-1; ++j) {
            block[j] = (char) ((rand() % 24) + 97);
        }
        block[source.record_size-1] ='\n';
        write(handle, block, source.record_size);
    }

    close(handle);
};

void sort_sys(file source){
    char blok[source.record_size];
    char blok2[source.record_size];
    int we = open(source.file_name, O_RDWR);
    for (int i = 0; i < source.record_amount; ++i) {
        for (int j = i; j < source.record_amount; ++j) {
            lseek(we, i*source.record_size, SEEK_SET);
            int li = read(we, blok, sizeof(blok));
            lseek(we, j*source.record_size, SEEK_SET);
            li = read(we, blok2, sizeof(blok));
            if (blok[0] > blok2[0]) {
                write(we, blok, li);
                lseek(we, i*source.record_size, SEEK_SET);
                write(we, blok2, li);
            }
        }
    }
};

void sort_lib(file source){

};

void copy_sys(file source, file target){
    char blok[1024];
    int we, wy;
    int liczyt;
    we=open(source.file_name, O_RDONLY);
    wy=open(target.file_name,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
    while((liczyt=read(we,blok,sizeof(blok)))>0)
        write(wy,blok,liczyt);
};

void copy_lib(file source, file target){
    char napis[1];
    FILE *plik = fopen(source.file_name, "r");
    FILE *plik2 = fopen(target.file_name, "w");
    if(plik && plik2)
    {
        for (int i = 0; i < source.record_amount * (source.record_size); ++i) {
            fread(napis, sizeof(char), 1,plik);
            fwrite(napis, sizeof(char), 1, plik2);
        }

        fclose(plik);
        fclose(plik2);
    }
};