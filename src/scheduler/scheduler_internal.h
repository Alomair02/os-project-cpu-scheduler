#ifndef SCHEDULER_INTERNAL_H
#define SCHEDULER_INTERNAL_H

#include "scheduler.h"

typedef struct ReadyNode {
    Process           *process;
    unsigned long      ready_seq;
    struct ReadyNode  *next;
} ReadyNode;

typedef struct {
    ReadyNode      *head;
    ReadyNode      *tail;
    unsigned long   next_ready_seq;
    Algorithm       active_algo;
} SchedulerState;

extern SchedulerState g_scheduler;

ReadyNode *scheduler_node_create(Process *p);
void       scheduler_clear(void);
void       scheduler_push_back(Process *p);
void       scheduler_insert_ordered(Process *p,
                                    int (*comes_before)(const ReadyNode *lhs,
                                                        const ReadyNode *rhs));
Process   *scheduler_pop_front(void);
int        scheduler_queue_is_empty(void);
int        scheduler_compare_seq_pid(const ReadyNode *lhs, const ReadyNode *rhs);

void fcfs_enqueue(Process *p);
void sjf_enqueue(Process *p);
void rr_enqueue(Process *p);
void priority_enqueue(Process *p);

#endif
