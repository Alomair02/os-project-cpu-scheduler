#include <stdio.h>
#include <stdlib.h>

#include "scheduler_internal.h"

SchedulerState g_scheduler = {0};

static void scheduler_validate_rr_quantum(const SimulatorState *s) {
    if (s->algo == ALGO_RR && s->quantum <= 0) {
        fprintf(stderr, "sched_init: RR requires a positive quantum\n");
        exit(EXIT_FAILURE);
    }
}

ReadyNode *scheduler_node_create(Process *p) {
    ReadyNode *node = malloc(sizeof(*node));

    if (node == NULL) {
        fprintf(stderr, "scheduler_node_create: out of memory\n");
        exit(EXIT_FAILURE);
    }

    node->process   = p;
    node->ready_seq = g_scheduler.next_ready_seq++;
    node->next      = NULL;
    return node;
}

void scheduler_clear(void) {
    ReadyNode *node = g_scheduler.head;

    while (node != NULL) {
        ReadyNode *next = node->next;
        free(node);
        node = next;
    }

    g_scheduler.head           = NULL;
    g_scheduler.tail           = NULL;
    g_scheduler.next_ready_seq = 0;
}

void scheduler_push_back(Process *p) {
    ReadyNode *node = scheduler_node_create(p);

    if (g_scheduler.tail == NULL) {
        g_scheduler.head = node;
        g_scheduler.tail = node;
        return;
    }

    g_scheduler.tail->next = node;
    g_scheduler.tail       = node;
}

int scheduler_compare_seq_pid(const ReadyNode *lhs, const ReadyNode *rhs) {
    if (lhs->ready_seq != rhs->ready_seq) {
        return (lhs->ready_seq < rhs->ready_seq) ? -1 : 1;
    }

    if (lhs->process->pid != rhs->process->pid) {
        return (lhs->process->pid < rhs->process->pid) ? -1 : 1;
    }

    return 0;
}

void scheduler_insert_ordered(Process *p,
                              int (*comes_before)(const ReadyNode *lhs,
                                                  const ReadyNode *rhs)) {
    ReadyNode *node = scheduler_node_create(p);

    if (g_scheduler.head == NULL || comes_before(node, g_scheduler.head)) {
        node->next       = g_scheduler.head;
        g_scheduler.head = node;

        if (g_scheduler.tail == NULL) {
            g_scheduler.tail = node;
        }
        return;
    }

    ReadyNode *current = g_scheduler.head;

    while (current->next != NULL && !comes_before(node, current->next)) {
        current = current->next;
    }

    node->next    = current->next;
    current->next = node;

    if (node->next == NULL) {
        g_scheduler.tail = node;
    }
}

Process *scheduler_pop_front(void) {
    ReadyNode *node;
    Process   *process;

    if (g_scheduler.head == NULL) {
        return NULL;
    }

    node             = g_scheduler.head;
    g_scheduler.head = node->next;

    if (g_scheduler.head == NULL) {
        g_scheduler.tail = NULL;
    }

    process = node->process;
    free(node);
    return process;
}

int scheduler_queue_is_empty(void) {
    return g_scheduler.head == NULL;
}

void sched_init(SimulatorState *s) {
    scheduler_clear();
    g_scheduler.active_algo = s->algo;
    scheduler_validate_rr_quantum(s);
}

void sched_enqueue(SimulatorState *s, Process *p) {
    g_scheduler.active_algo = s->algo;

    switch (s->algo) {
    case ALGO_FCFS:
        fcfs_enqueue(p);
        break;
    case ALGO_SJF:
        sjf_enqueue(p);
        break;
    case ALGO_RR:
        rr_enqueue(p);
        break;
    case ALGO_PRIORITY:
        priority_enqueue(p);
        break;
    default:
        fprintf(stderr, "sched_enqueue: unsupported algorithm %d\n", s->algo);
        exit(EXIT_FAILURE);
    }
}

Process *sched_next(SimulatorState *s) {
    g_scheduler.active_algo = s->algo;
    return scheduler_pop_front();
}

int sched_is_empty(SimulatorState *s) {
    g_scheduler.active_algo = s->algo;
    return scheduler_queue_is_empty();
}
