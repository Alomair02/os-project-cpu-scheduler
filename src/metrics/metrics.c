#include <stdio.h>
#include <stdlib.h>
#include "metrics.h"
#include "../output/gantt.h"

extern GanttChart *g_gantt;

void metrics_record(SimulatorState *s, Event *e)
{
    Process *p = e->process;

    switch (e->type) {

    case EVENT_CPU_DONE:
        /* Record the CPU slice that just finished */
        if (g_gantt) {
            int burst = p->cpu_bursts[p->current_burst_index];
            gantt_record(g_gantt, p->pid, s->current_time - burst, s->current_time);
        }
        if (p->finish_time < 0)
            p->finish_time = s->current_time;
        break;

    case EVENT_PREEMPTION:
        /* Record the quantum slice that just got preempted */
        if (g_gantt) {
            gantt_record(g_gantt, p->pid,
                         s->current_time - s->quantum, s->current_time);
        }
        break;

    case EVENT_TERMINATE:
        if (p->finish_time < 0)
            p->finish_time = s->current_time;
        break;

    default:
        break;
    }
}

void metrics_print(SimulatorState *s)
{
    int n = s->num_processes;

    double total_wait       = 0.0;
    double total_turnaround = 0.0;
    double total_response   = 0.0;

    printf("\n");
    printf("============================================================\n");
    printf("                   PER-PROCESS STATISTICS                   \n");
    printf("============================================================\n");
    printf("%-6s %-10s %-12s %-12s %-12s\n",
           "PID", "Arrival", "Response", "Waiting", "Turnaround");
    printf("%-6s %-10s %-12s %-12s %-12s\n",
           "---", "-------", "--------", "-------", "----------");

    for (int i = 0; i < n; i++) {
        Process *p = s->processes[i];

        int response_time   = (p->start_time  >= 0)
                              ? p->start_time  - p->arrival_time : -1;
        int turnaround_time = (p->finish_time >= 0)
                              ? p->finish_time - p->arrival_time : -1;
        int wait_time       = p->waiting_time;

        if (response_time   >= 0) total_response   += response_time;
        if (wait_time       >= 0) total_wait       += wait_time;
        if (turnaround_time >= 0) total_turnaround += turnaround_time;

        printf("%-6d %-10d %-12d %-12d %-12d\n",
               p->pid, p->arrival_time,
               response_time, wait_time, turnaround_time);
    }

    int sim_end = 0;
    for (int i = 0; i < n; i++)
        if (s->processes[i]->finish_time > sim_end)
            sim_end = s->processes[i]->finish_time;

    double cpu_utilization = (sim_end > 0)
        ? ((double)s->cpu_busy_time / sim_end) * 100.0 : 0.0;

    double throughput = (sim_end > 0)
        ? (double)n / sim_end : 0.0;

    printf("\n");
    printf("============================================================\n");
    printf("                   SYSTEM-WIDE STATISTICS                   \n");
    printf("============================================================\n");
    printf("  Total processes          : %d\n",   n);
    printf("  Simulation end time      : %d\n",   sim_end);
    printf("  CPU busy time            : %d\n",   s->cpu_busy_time);
    printf("  CPU utilization          : %.2f%%\n", cpu_utilization);
    printf("  Throughput               : %.4f proc/unit\n", throughput);
    printf("  Context switches         : %d\n",   s->context_switch_count);
    printf("  Avg response time        : %.2f\n", total_response   / n);
    printf("  Avg waiting time         : %.2f\n", total_wait       / n);
    printf("  Avg turnaround time      : %.2f\n", total_turnaround / n);
    printf("============================================================\n");
}
