#include <stdio.h>
#include <stdlib.h>
#include "include/process.h"
#include "engine/simulator.h"

/* =========================================
 * MOCKS FOR FERAS (Scheduler logic)
 * ========================================= */
Process *mock_ready_queue[10];
int mock_rq_head = 0, mock_rq_tail = 0;

void sched_init(SimulatorState *s) {
    printf("[Mock] Scheduler initialized.\n");
}

void sched_enqueue(SimulatorState *s, Process *p) {
    mock_ready_queue[mock_rq_tail++] = p;
}

Process *sched_next(SimulatorState *s) {
    return mock_ready_queue[mock_rq_head++];
}

int sched_is_empty(SimulatorState *s) {
    return mock_rq_head == mock_rq_tail;
}

/* =========================================
 * MOCKS FOR FAISAL (Metrics logic)
 * ========================================= */
void metrics_record(SimulatorState *s, Event *e) {
    // This perfectly logs your event loop's behavior!
    const char *event_names[] = {
        "ARRIVAL", "CPU_DISPATCH", "CPU_DONE", 
        "IO_START", "IO_DONE", "PREEMPTION", "TERMINATE"
    };
    printf("Clock: %3d | Event: %-12s | PID: %d\n", 
           s->current_time, event_names[e->type], e->process->pid);
}

/* =========================================
 * INDEPENDENT ENGINE TEST
 * ========================================= */
int main() {
    printf("--- Starting Independent Engine Test ---\n\n");

    // 1. Manually create a dummy process
    // It arrives at t=0, needs 5 units of CPU, 3 of I/O, then 4 of CPU.
    Process *p1 = malloc(sizeof(Process));
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

    // 2. Initialize simulator (Testing FCFS, quantum 0)
    SimulatorState *sim = sim_create(procs, 1, ALGO_FCFS, 0);

    // 3. Run the engine!
    sim_run(sim);

    // 4. Clean up
    sim_destroy(sim);
    free(p1);

    printf("\n--- Test Complete ---\n");
    return 0;
}