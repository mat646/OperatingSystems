//
// Created by Mateusz Sokol on 19.05.18.
//

#define _GNU_SOURCE
#define MAX_SIZE 1000

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <zconf.h>
#include <math.h>
#include <sys/param.h>

struct arg_struct {
    int offset;
    int row_amount;
};

int c, height, width, max;
int input_image[MAX_SIZE][MAX_SIZE];
int output_image[MAX_SIZE][MAX_SIZE];
int filter[MAX_SIZE][MAX_SIZE];

void *compute_img(struct arg_struct *args);

int main(int argc, char **argv) {

    if (argc == 1 || argv[1] == NULL || argv[2] == NULL || argv[3] == NULL || argv[4] == NULL) {
        printf("Parametry wywolania:\n 1. liczba watkow\n 2. obraz wejsciowy\n 3. definicja filtru\n 4. plik wynikowy\n");
        return 0;
    }


    int N;
    sscanf(argv[1], "%d", &N);
    char input_img_path[20], filter_path[20], output_path[20];

    sscanf(argv[2], "%s", input_img_path);
    sscanf(argv[3], "%s", filter_path);
    sscanf(argv[4], "%s", output_path);

    FILE *file = fopen(input_img_path, "r");

    char napis[100];

    if (file) { //TODO reading from file
        for (int i = 0; i < 100; ++i) {
            fread(napis, sizeof(char), 100, file);
        }

        fclose(file);
    }


    //----------------------

    pthread_t inc_x_thread;
    //int x = 0;

    for (int i = 0; i < N; ++i) {

        struct arg_struct args = args;

        args.offset = 1;
        args.row_amount = 2;

        if (pthread_create(&inc_x_thread, NULL, (void *(*)(void *)) compute_img, &args)) {
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }

        //sleep(1);

        if (pthread_join(inc_x_thread, NULL)) {

            fprintf(stderr, "Error joining thread\n");
            return 2;

        }

        printf("%d\n", c);

    }


}

double compute_cell(int x, int y) {
    double s = 0;

    for (int i = 0; i < c; ++i) {
        for (int j = 0; j < c; ++j) {

            MIN(2, 2);
            s += input_image[MAX(1, x - (int) ceil((double) c / 2) + i)][MAX(1, y - (int) ceil((double) c / 2) + j)] *
                 filter[i][j];
        }
    }

    return s;
}

void *compute_img(struct arg_struct *args) {

    for (int i = args->offset; i < args->offset + args->row_amount; ++i) {
        for (int j = 0; j < width; ++j) {
            //output_image[i][j] = (int) round(compute_cell(i, j));
        }
    }

    printf("%ld finished computing\n", (unsigned long int) pthread_self());

    return 0;
}
