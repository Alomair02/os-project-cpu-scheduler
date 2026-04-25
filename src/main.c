#include <stdio.h>
#include <stdlib.h>
#include "include/process.h"
#include "engine/simulator.h"

/* Temporary Phase 2 metrics hook.
 * The final metrics module will calculate summary statistics in Phase 3.
 */
void metrics_record(SimulatorState *s, Event *e) {
    const char *event_names[] = {
        "ARRIVAL", "CPU_DISPATCH", "CPU_DONE",
        "IO_START", "IO_DONE", "PREEMPTION", "TERMINATE"
    };
    printf("Clock: %3d | Event: %-12s | PID: %d\n",
           s->current_time, event_names[e->type], e->process->pid);
}

int main(void) {
    printf("--- Starting Phase 2 CPU Scheduler Demo ---\n\n");

    /* Workload parsing is Phase 3 work. This demo uses one process with
     * two CPU bursts and one I/O burst to exercise the simulator path.
     */
    Process *p1 = calloc(1, sizeof(*p1));
    if (p1 == NULL) {
        fprintf(stderr, "Unable to allocate demo process\n");
        return EXIT_FAILURE;
    }

    p1->pid = 1;
    p1->arrival_time = 0;
    p1->priority = 1;

    p1->num_cpu_bursts = 2;
    p1->num_io_bursts = 1;

    p1->cpu_bursts[0] = 5;
    p1->io_bursts[0] = 3;
    p1->cpu_bursts[1] = 4;

    p1->current_burst_index = 0;
    p1->state = STATE_NEW;
    p1->response_time_set = 0;

    Process *procs[] = { p1 };

    SimulatorState *sim = sim_create(procs, 1, ALGO_FCFS, 0);
    sim_run(sim);

    sim_destroy(sim);
    free(p1);

    printf("\n--- Demo Complete ---\n");
    return EXIT_SUCCESS;
}
