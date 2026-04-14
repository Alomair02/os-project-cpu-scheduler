#ifndef PROCESS_H
#define PROCESS_H

#include "types.h"

#define MAX_BURSTS 64

typedef struct {
    int pid;
    int arrival_time;
    int priority;

    int cpu_bursts[MAX_BURSTS];
    int io_bursts[MAX_BURSTS];
    int num_cpu_bursts;
    int num_io_bursts;

    int current_burst_index;   /* which CPU burst we're on */

    ProcessState state;

    /* metrics — filled in as simulation runs */
    int start_time;            /* first time on CPU */
    int finish_time;
    int waiting_time;
    int response_time_set;     /* flag: 0 until first dispatch */
} Process;

#endif