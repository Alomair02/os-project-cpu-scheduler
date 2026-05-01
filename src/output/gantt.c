#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gantt.h"

#define CELL_WIDTH 4

GanttChart *gantt_create(void)
{
    GanttChart *g = malloc(sizeof(GanttChart));
    if (!g) {
        fprintf(stderr, "[gantt] ERROR: out of memory\n");
        return NULL;
    }
    g->num_slots = 0;
    return g;
}

void gantt_destroy(GanttChart *g)
{
    free(g);
}

void gantt_record(GanttChart *g, int pid, int start_time, int end_time)
{
    if (!g) return;

    if (g->num_slots >= GANTT_MAX_SLOTS) {
        fprintf(stderr, "[gantt] WARNING: chart full, dropping slot.\n");
        return;
    }

    if (g->num_slots > 0) {
        GanttSlot *last = &g->slots[g->num_slots - 1];
        if (last->pid == pid && last->end_time == start_time) {
            last->end_time = end_time;
            return;
        }
    }

    g->slots[g->num_slots].pid        = pid;
    g->slots[g->num_slots].start_time = start_time;
    g->slots[g->num_slots].end_time   = end_time;
    g->num_slots++;
}

void gantt_print(GanttChart *g)
{
    if (!g || g->num_slots == 0) {
        printf("[gantt] No data to display.\n");
        return;
    }

    printf("\n");
    printf("============================================================\n");
    printf("                       GANTT CHART                         \n");
    printf("============================================================\n");

    for (int i = 0; i < g->num_slots; i++) {
        char label[16];
        if (g->slots[i].pid == GANTT_IDLE)
            snprintf(label, sizeof(label), "IDLE");
        else
            snprintf(label, sizeof(label), "P%-3d", g->slots[i].pid);
        printf("|%-*.*s", CELL_WIDTH, CELL_WIDTH, label);
    }
    printf("|\n");

    for (int i = 0; i < g->num_slots; i++)
        printf("%-*d", CELL_WIDTH + 1, g->slots[i].start_time);
    printf("%d\n", g->slots[g->num_slots - 1].end_time);

    printf("============================================================\n");
}
