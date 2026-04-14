#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "../include/types.h"
#include "../include/process.h"
#include "event_queue.h"

typedef struct {
    Process   **processes;
    int         num_processes;
    Algorithm   algo;
    int         quantum;          /* RR only */
    EventQueue *event_queue;
    int         current_time;
    int         context_switch_count;
    int         cpu_busy_time;
} SimulatorState;

SimulatorState *sim_create(Process **procs, int n, Algorithm algo, int quantum);
void            sim_run(SimulatorState *s);
void            sim_destroy(SimulatorState *s);

#endif