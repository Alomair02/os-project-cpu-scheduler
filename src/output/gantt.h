#ifndef GANTT_H
#define GANTT_H

#define GANTT_MAX_SLOTS 1024
#define GANTT_IDLE -1

typedef struct {
    int pid;
    int start_time;
    int end_time;
} GanttSlot;

typedef struct {
    GanttSlot slots[GANTT_MAX_SLOTS];
    int       num_slots;
} GanttChart;

GanttChart *gantt_create(void);
void        gantt_destroy(GanttChart *g);
void        gantt_record(GanttChart *g, int pid, int start_time, int end_time);
void        gantt_print(GanttChart *g);

#endif /* GANTT_H */
