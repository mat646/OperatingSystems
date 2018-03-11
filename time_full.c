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

    Time start_time;
    start_time.real_time = rtsp.tv_nsec + rtsp.tv_sec*1000000000;

    struct tms mtms;
    times(&mtms);

    start_time.system_time = mtms.tms_stime;
    start_time.user_time = mtms.tms_utime;

    return start_time;
}

void end(Time *start_time) {
    struct tms mtms;
    times(&mtms);
    struct timespec rtsp;
    clock_gettime(CLOCK_REALTIME, &rtsp);

    start_time->real_time = (rtsp.tv_nsec + rtsp.tv_sec*1000000000) - start_time->real_time;
    start_time->system_time = mtms.tms_stime - start_time->system_time;
    start_time->user_time = mtms.tms_utime - start_time->user_time;
}

void print(Time time1, char *label) {
    printf("\n%s\n", label);
    printf("user time: %f\n", (float)(time1.user_time * 10000000.0) / CLOCKS_PER_SEC);
    printf("system time: %f\n", (float)(time1.system_time * 1000000.0) / CLOCKS_PER_SEC);
    printf("real time: %f\n", (float)time1.real_time/1000000);
}