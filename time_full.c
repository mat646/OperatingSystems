//
// Created by mateusz on 10.03.18.
//

#include <time.h>
#include <sys/times.h>
#include "time_full.h"

Time start() {
    struct timespec rtsp;
    clock_gettime(CLOCK_REALTIME, &rtsp);
    struct tms mtms;
    times(&mtms);

    Time start_time;
    start_time.real_time = rtsp.tv_sec;
    start_time.system_time = mtms.tms_stime;
    start_time.user_time = mtms.tms_utime;

    return start_time;
}

void end(Time *start_time) {
    struct timespec rtsp;
    clock_gettime(CLOCK_REALTIME, &rtsp);
    struct tms mtms;
    times(&mtms);

    start_time->real_time = rtsp.tv_sec - start_time->real_time;
    start_time->system_time = mtms.tms_stime - start_time->system_time;
    start_time->user_time = mtms.tms_utime - start_time->user_time;
}