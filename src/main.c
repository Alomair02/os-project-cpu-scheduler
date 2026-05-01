#include <stdio.h>
#include <stdlib.h>

#include "include/process.h"
#include "include/types.h"
#include "engine/simulator.h"
#include "io/io_handler.h"
#include "output/output.h"
#include "output/gantt.h"

GanttChart *g_gantt = NULL;

void     sched_init    (SimulatorState *s);
void     sched_enqueue (SimulatorState *s, Process *p);
Process *sched_next    (SimulatorState *s);
int      sched_is_empty(SimulatorState *s);

int main(int argc, char *argv[])
{
    char     *filepath = NULL;
    Algorithm algo     = ALGO_FCFS;
    int       quantum  = 4;

    if (parse_args(argc, argv, &filepath, &algo, &quantum) != 0)
        return EXIT_FAILURE;

    Process **procs = NULL;
    int n = load_workload(filepath, &procs);
    if (n < 0)
        return EXIT_FAILURE;

    SimulatorState *sim = sim_create(procs, n, algo, quantum);
    if (!sim) {
        fprintf(stderr, "ERROR: sim_create() failed\n");
        return EXIT_FAILURE;
    }

    g_gantt = gantt_create();

    print_header(sim);

    sim_run(sim);

    print_results(sim, g_gantt);

    gantt_destroy(g_gantt);
    sim_destroy(sim);

    for (int i = 0; i < n; i++)
        free(procs[i]);
    free(procs);

    return EXIT_SUCCESS;
}
