#include <stdio.h>
#include <stdlib.h>
#include "simulator.h"

/* -- Hooks implemented by Feras in scheduler/ -- */
extern void     sched_init(SimulatorState *s);
extern void     sched_enqueue(SimulatorState *s, Process *p);
extern Process *sched_next(SimulatorState *s);
extern int      sched_is_empty(SimulatorState *s);

/* -- Hook implemented by Faisal in metrics/ -- */
extern void metrics_record(SimulatorState *s, Event *e);

SimulatorState *sim_create(Process **procs, int n, Algorithm algo, int quantum) {
    SimulatorState *s = malloc(sizeof(SimulatorState));
    s->processes           = procs;
    s->num_processes       = n;
    s->algo                = algo;
    s->quantum             = quantum;
    s->event_queue         = eq_create(256);
    s->current_time        = 0;
    s->context_switch_count = 0;
    s->cpu_busy_time        = 0;
    return s;
}

static void dispatch_next(SimulatorState *s) {
    if (sched_is_empty(s)) return;
    Process *p = sched_next(s);
    p->state = STATE_RUNNING;

    /* record response time on first dispatch */
    if (!p->response_time_set) {
        /* response_time = current_time - arrival_time; stored by metrics */
        p->response_time_set = 1;
        p->start_time = s->current_time;
    }

    int burst = p->cpu_bursts[p->current_burst_index];
    Event e;

    if (s->algo == ALGO_RR && burst > s->quantum) {
        e.timestamp = s->current_time + s->quantum;
        e.type      = EVENT_PREEMPTION;
    } else {
        e.timestamp = s->current_time + burst;
        e.type      = EVENT_CPU_DONE;
    }
    e.process = p;
    eq_insert(s->event_queue, e);
}

void sim_run(SimulatorState *s) {
    sched_init(s);

    /* seed arrival events */
    for (int i = 0; i < s->num_processes; i++) {
        Event e = {
            .timestamp = s->processes[i]->arrival_time,
            .type      = EVENT_ARRIVAL,
            .process   = s->processes[i]
        };
        eq_insert(s->event_queue, e);
    }

    int cpu_free = 1;

    while (!eq_is_empty(s->event_queue)) {
        Event e = eq_pop(s->event_queue);
        s->current_time = e.timestamp;
        metrics_record(s, &e);

        switch (e.type) {

        case EVENT_ARRIVAL:
            e.process->state = STATE_READY;
            sched_enqueue(s, e.process);
            if (cpu_free) { cpu_free = 0; dispatch_next(s); }
            break;

        case EVENT_CPU_DONE: {
            Process *p = e.process;
            s->cpu_busy_time += p->cpu_bursts[p->current_burst_index];
            int has_io = p->current_burst_index < p->num_io_bursts;

            if (has_io) {
                p->state = STATE_WAITING;
                int io_dur = p->io_bursts[p->current_burst_index];
                Event io_done = {
                    .timestamp = s->current_time + io_dur,
                    .type      = EVENT_IO_DONE,
                    .process   = p
                };
                eq_insert(s->event_queue, io_done);
            } else {
                p->state      = STATE_TERMINATED;
                p->finish_time = s->current_time;
            }

            s->context_switch_count++;
            s->current_time += CONTEXT_SWITCH_OVERHEAD;

            if (!sched_is_empty(s)) { dispatch_next(s); }
            else                    { cpu_free = 1; }
            break;
        }

        case EVENT_IO_DONE:
            e.process->current_burst_index++;
            e.process->state = STATE_READY;
            sched_enqueue(s, e.process);
            if (cpu_free) { cpu_free = 0; dispatch_next(s); }
            break;

        case EVENT_PREEMPTION: {
            Process *p = e.process;
            s->cpu_busy_time += s->quantum;
            /* shorten the remaining burst */
            p->cpu_bursts[p->current_burst_index] -= s->quantum;
            p->state = STATE_READY;
            s->context_switch_count++;
            s->current_time += CONTEXT_SWITCH_OVERHEAD;
            sched_enqueue(s, p);
            dispatch_next(s);
            break;
        }

        default:
            break;
        }
    }
}

void sim_destroy(SimulatorState *s) {
    eq_destroy(s->event_queue);
    free(s);
}