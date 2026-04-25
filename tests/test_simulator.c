#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/include/process.h"
#include "../src/engine/simulator.h"

typedef struct {
    EventType type;
    int       pid;
    int       timestamp;
} EventLogEntry;

static EventLogEntry g_events[128];
static int           g_event_count = 0;

void metrics_record(SimulatorState *s, Event *e) {
    if (g_event_count < (int)(sizeof(g_events) / sizeof(g_events[0]))) {
        g_events[g_event_count].type      = e->type;
        g_events[g_event_count].pid       = e->process->pid;
        g_events[g_event_count].timestamp = s->current_time;
        g_event_count++;
    }
}

static void reset_event_log(void) {
    memset(g_events, 0, sizeof(g_events));
    g_event_count = 0;
}

static Process *alloc_process(int pid,
                              int arrival_time,
                              int priority,
                              const int *cpu_bursts,
                              int num_cpu_bursts,
                              const int *io_bursts,
                              int num_io_bursts) {
    Process *p = malloc(sizeof(*p));

    assert(p != NULL);
    memset(p, 0, sizeof(*p));

    p->pid            = pid;
    p->arrival_time   = arrival_time;
    p->priority       = priority;
    p->num_cpu_bursts = num_cpu_bursts;
    p->num_io_bursts  = num_io_bursts;

    if (num_cpu_bursts > 0) {
        memcpy(p->cpu_bursts, cpu_bursts, sizeof(int) * num_cpu_bursts);
    }

    if (num_io_bursts > 0) {
        memcpy(p->io_bursts, io_bursts, sizeof(int) * num_io_bursts);
    }

    return p;
}

static void destroy_processes(Process **procs, int count) {
    int i;

    for (i = 0; i < count; i++) {
        free(procs[i]);
    }
}

static int count_events(EventType type) {
    int count = 0;
    int i;

    for (i = 0; i < g_event_count; i++) {
        if (g_events[i].type == type) {
            count++;
        }
    }

    return count;
}

static void test_sjf_is_non_preemptive(void) {
    int cpu1[] = {10};
    int cpu2[] = {1};
    Process *p1 = alloc_process(1, 0, 5, cpu1, 1, NULL, 0);
    Process *p2 = alloc_process(2, 1, 1, cpu2, 1, NULL, 0);
    Process *procs[] = {p1, p2};
    SimulatorState *sim;

    reset_event_log();
    sim = sim_create(procs, 2, ALGO_SJF, 0);
    sim_run(sim);

    assert(p1->start_time == 0);
    assert(p1->finish_time == 10);
    assert(p2->start_time == 11);
    assert(p2->finish_time == 12);
    assert(count_events(EVENT_PREEMPTION) == 0);

    sim_destroy(sim);
    destroy_processes(procs, 2);
}

static void test_rr_rotates_processes(void) {
    int cpu1[] = {5};
    int cpu2[] = {5};
    Process *p1 = alloc_process(10, 0, 4, cpu1, 1, NULL, 0);
    Process *p2 = alloc_process(11, 0, 4, cpu2, 1, NULL, 0);
    Process *procs[] = {p1, p2};
    SimulatorState *sim;

    reset_event_log();
    sim = sim_create(procs, 2, ALGO_RR, 2);
    sim_run(sim);

    assert(p1->start_time == 0);
    assert(p2->start_time == 3);
    assert(p1->finish_time == 13);
    assert(p2->finish_time == 15);
    assert(count_events(EVENT_PREEMPTION) == 4);
    assert(sim->context_switch_count == 6);

    sim_destroy(sim);
    destroy_processes(procs, 2);
}

static void test_priority_is_non_preemptive(void) {
    int cpu1[] = {4};
    int cpu2[] = {1};
    Process *p1 = alloc_process(20, 0, 5, cpu1, 1, NULL, 0);
    Process *p2 = alloc_process(21, 1, 1, cpu2, 1, NULL, 0);
    Process *procs[] = {p1, p2};
    SimulatorState *sim;

    reset_event_log();
    sim = sim_create(procs, 2, ALGO_PRIORITY, 0);
    sim_run(sim);

    assert(p1->start_time == 0);
    assert(p1->finish_time == 4);
    assert(p2->start_time == 5);
    assert(p2->finish_time == 6);
    assert(count_events(EVENT_PREEMPTION) == 0);

    sim_destroy(sim);
    destroy_processes(procs, 2);
}

static void test_io_reentry_reuses_scheduler(void) {
    int cpu1[] = {3, 4};
    int io1[] = {2};
    int cpu2[] = {2};
    Process *p1 = alloc_process(30, 0, 5, cpu1, 2, io1, 1);
    Process *p2 = alloc_process(31, 1, 1, cpu2, 1, NULL, 0);
    Process *procs[] = {p1, p2};
    SimulatorState *sim;

    reset_event_log();
    sim = sim_create(procs, 2, ALGO_FCFS, 0);
    sim_run(sim);

    assert(p1->start_time == 0);
    assert(p2->start_time == 4);
    assert(p2->finish_time == 6);
    assert(p1->finish_time == 11);
    assert(p1->current_burst_index == 1);
    assert(count_events(EVENT_IO_DONE) == 1);

    sim_destroy(sim);
    destroy_processes(procs, 2);
}

int main(void) {
    printf("--- Running Simulator + Scheduler Tests ---\n");

    test_sjf_is_non_preemptive();
    test_rr_rotates_processes();
    test_priority_is_non_preemptive();
    test_io_reentry_reuses_scheduler();

    printf("Simulator integration tests passed.\n");
    return 0;
}
