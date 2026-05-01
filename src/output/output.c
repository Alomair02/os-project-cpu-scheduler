#include <stdio.h>
#include "output.h"
#include "../metrics/metrics.h"

void print_header(SimulatorState *s)
{
    const char *algo_names[] = {
        "First-Come First-Served (FCFS)",
        "Shortest Job First (SJF)",
        "Round Robin (RR)",
        "Priority Scheduling"
    };

    printf("\n");
    printf("############################################################\n");
    printf("#          CPU SCHEDULING SIMULATOR — Phase 3              #\n");
    printf("############################################################\n");
    printf("  Algorithm  : %s\n", algo_names[s->algo]);
    if (s->algo == ALGO_RR)
        printf("  Quantum    : %d\n", s->quantum);
    printf("  Processes  : %d\n", s->num_processes);
    printf("############################################################\n");
    printf("\n");
}

void print_results(SimulatorState *s, GanttChart *chart)
{
    if (chart)
        gantt_print(chart);

    metrics_print(s);
}
