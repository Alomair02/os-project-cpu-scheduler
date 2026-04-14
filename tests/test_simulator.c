#include <stdio.h>
#include <stdlib.h>
#include "../src/include/process.h"
#include "../src/engine/simulator.h"

/* --- MOCKS --- */
Process *mock_rq[10];
int mock_head = 0, mock_tail = 0;

void sched_init(SimulatorState *s) {}
void sched_enqueue(SimulatorState *s, Process *p) { mock_rq[mock_tail++] = p; }
Process *sched_next(SimulatorState *s) { return mock_rq[mock_head++]; }
int sched_is_empty(SimulatorState *s) { return mock_head == mock_tail; }

void metrics_record(SimulatorState *s, Event *e) {
    const char *names[] = {"ARRIVAL", "CPU_DISP", "CPU_DONE", "IO_START", "IO_DONE", "PREEMPT", "TERM"};
    printf("t=%3d | %-10s | PID: %d\n", s->current_time, names[e->type], e->process->pid);
}
/* ------------- */

int main() {
    printf("--- Running Simulator Core Test ---\n");

    // Setup a dummy process
    Process *p1 = malloc(sizeof(Process));
    p1->pid = 100;
    p1->arrival_time = 0;
    p1->num_cpu_bursts = 2;
    p1->num_io_bursts = 1;
    p1->cpu_bursts[0] = 5;
    p1->io_bursts[0] = 3;
    p1->cpu_bursts[1] = 4;
    p1->current_burst_index = 0;
    p1->response_time_set = 0;

    Process *procs[] = { p1 };

    // Run simulator
    SimulatorState *sim = sim_create(procs, 1, ALGO_FCFS, 0);
    sim_run(sim);

    sim_destroy(sim);
    free(p1);
    
    printf("\nSimulator test finished! ✅\n");
    return 0;
}