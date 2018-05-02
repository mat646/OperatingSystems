//
// Created by Mateusz Sokol on 02.05.18.
//

#ifndef OPERATINGSYSTEMS_CONTRACT_H
#define OPERATINGSYSTEMS_CONTRACT_H

#define SHM_ID 100
#define SEM_ID 200
#define MAX_QUEUE_LEN 1024
#define _GNU_SOURCE

struct barber_shop {
    int queue[MAX_QUEUE_LEN]; // -1 - empty seat
    char barber_state; // s - sleeping, w - working
    int queue_start, queue_end, queue_len;
    int on_chair;
};

#endif //OPERATINGSYSTEMS_CONTRACT_H
