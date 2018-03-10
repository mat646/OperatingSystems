//
// Created by mateusz on 10.03.18.
//

#ifndef SETONE_TIME_FULL_H
#define SETONE_TIME_FULL_H

typedef struct Time {
    float real_time;
    float user_time;
    float system_time;
} Time;

Time start();

void end(Time *start_time);

#endif //SETONE_TIME_FULL_H
