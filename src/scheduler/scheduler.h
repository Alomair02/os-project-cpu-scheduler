#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../include/process.h"
#include "../engine/simulator.h"

/* * Core hooks called by Abdulaziz's simulation engine.
 * Feras needs to implement these to route processes to the correct 
 * algorithm (FCFS, SJF, RR, Priority) based on s->algo.
 */
void     sched_init(SimulatorState *s);
void     sched_enqueue(SimulatorState *s, Process *p);
Process *sched_next(SimulatorState *s);
int      sched_is_empty(SimulatorState *s);

#endif