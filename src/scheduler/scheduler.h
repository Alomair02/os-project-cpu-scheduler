#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../include/process.h"
#include "../engine/simulator.h"

/* Core hooks called by the simulation engine.
 * The scheduler routes processes using s->algo and manages the ready queue.
 */
void     sched_init(SimulatorState *s);
void     sched_enqueue(SimulatorState *s, Process *p);
Process *sched_next(SimulatorState *s);
int      sched_is_empty(SimulatorState *s);

#endif
