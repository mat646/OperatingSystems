//
// Created by Mateusz Sokol on 19.05.18.
//

#define _GNU_SOURCE
#define MAX_SIZE 1000

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <sys/param.h>

struct arg_struct {
    int offset;
    int row_amount;
};

int c, height, width, max;
int input_image[MAX_SIZE][MAX_SIZE];
int output_image[MAX_SIZE][MAX_SIZE];
double filter[MAX_SIZE][MAX_SIZE];

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

    char type[2];

    if (file) {

        fscanf(file, "%s\n", type);
        fscanf(file, "%d", &width);
        fscanf(file, "%d", &height);
        fscanf(file, "%d", &max);

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; ++j) {
                fscanf(file, "%d", &input_image[i][j]);
            }
        }

        fclose(file);
    } else {
        printf("can't open file image\n");
        exit(1);
    }

    file = fopen(filter_path, "r");

    if (file) {

        fscanf(file, "%d", &c);

        for (int i = 0; i < c; i++) {
            for (int j = 0; j < c; ++j) {
                fscanf(file, "%lf", &filter[i][j]);
            }
        }

        fclose(file);
    } else {
        printf("can't open file filter\n");
        exit(1);
    }

    //TODO time measurement

    pthread_t inc_x_thread;

    for (int i = 0; i < N; ++i) {

        struct arg_struct args;

        args.offset = i * (height / N);
        args.row_amount = height / N;

        if (pthread_create(&inc_x_thread, NULL, (void *(*)(void *)) compute_img, &args)) {
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }

        if (pthread_join(inc_x_thread, NULL)) {

            fprintf(stderr, "Error joining thread\n");
            return 2;

        }

    }

    sscanf(argv[4], "%s", output_path);

    file = fopen(output_path, "w");

    if (file) {

        fprintf(file, "P2\n%d %d\n%d\n", width, height, max);

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; ++j) {
                fprintf(file, "%d ", output_image[i][j]);
            }
            fprintf(file, "\n");
        }

        fclose(file);
    } else {
        printf("can't open file out\n");
        exit(1);
    }

}

double compute_cell(int x, int y) {

    double s = 0;

    for (int i = 0; i < c; ++i) {
        for (int j = 0; j < c; ++j) {
            s += (double) input_image[MAX(1, x - (int) ceil((double) c / 2.0) + i)][MAX(1, y - (int) ceil(
                    (double) c / 2.0) + j)] *
                 filter[i][j];
        }
    }

    return s;
}

void *compute_img(struct arg_struct *args) {

    for (int i = args->offset; i < args->offset + args->row_amount; ++i) {
        for (int j = 0; j < width; ++j) {
            output_image[i][j] = (int) round(compute_cell(i, j));
        }
    }

    printf("%ld finished computing %d rows\n", (unsigned long int) pthread_self(), args->offset);

    return 0;
}
