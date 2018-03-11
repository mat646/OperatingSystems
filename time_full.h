//
// Created by Mateusz Sokol on 10.03.18.
//

#ifndef SETONE_TIME_FULL_H
#define SETONE_TIME_FULL_H

typedef struct Time {
    __clock_t real_time;
    __clock_t user_time;
    __clock_t system_time;
} Time;

Time start();

void end(Time *start_time);

void print(Time time1, char *label);

#endif //SETONE_TIME_FULL_H
