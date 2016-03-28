#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "elapsedtime.h"

/**********************************************************************
 * One question you might ask after reading this and the makefile is, *
 * "Why put so much effort into clock_gettime()?"                     *
 *                                                                    *
 * Well, it turns out that gettimeofday() has a maximum resolution of *
 * microseconds and, more importantly, has no guarantee that the time *
 * will always increase, because it uses system time, which can be    *
 * changed by a system administrator. clock_gettime(), however, uses  *
 * the actual hardware time, so it will always increase between       *
 * subsequent calls.                                                  *
 **********************************************************************/

static struct timespec start_time;
static struct timespec end_time;

void start_clock(){
    if (clock_gettime(CLOCK_REALTIME, &start_time) == -1){
        fprintf(stderr, "Error getting starting clocktime");
        exit(1);
    }
}

void end_clock(){
    if (clock_gettime(CLOCK_REALTIME, &end_time) == -1){
        fprintf(stderr, "Error getting ending clock time");
        exit(1);
    }
}

long int get_time_elapsed(){
    return (end_time.tv_sec * 1000000000 + end_time.tv_nsec) - (start_time.tv_sec * 1000000000 + start_time.tv_nsec);
}
