//
// Created by Mateusz Sokol on 10.03.18.
//

#include <time.h>
#include <sys/times.h>
#include <stdio.h>
#include "time_full.h"

Time start() {
    struct timespec rtsp;
    clock_gettime(CLOCK_REALTIME, &rtsp);
    struct tms mtms;
    times(&mtms);

    Time start_time;
    start_time.real_time = rtsp.tv_nsec / 1000000;
    start_time.system_time = mtms.tms_stime;
    start_time.user_time = mtms.tms_utime;

    return start_time;
}

void end(Time *start_time) {
    struct timespec rtsp;
    clock_gettime(CLOCK_REALTIME, &rtsp);
    struct tms mtms;
    times(&mtms);

    float real_diff = (rtsp.tv_nsec / 1000000) - start_time->real_time;

    if (real_diff < 0) {
        start_time->real_time = 1000 + real_diff;
    } else {
        start_time->real_time = real_diff;
    }

    start_time->system_time = mtms.tms_stime - start_time->system_time;
    start_time->user_time = mtms.tms_utime - start_time->user_time;
}

void print(Time time1, char *label) {
    printf("\n%s\n", label);
    printf("user time: %f\n", time1.user_time);
    printf("system time: %f\n", time1.system_time);
    printf("real time: %f\n", time1.real_time);
}