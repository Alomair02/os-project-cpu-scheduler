#ifndef TYPES_H
#define TYPES_H

typedef enum {
    EVENT_ARRIVAL,
    EVENT_CPU_DISPATCH,
    EVENT_CPU_DONE,
    EVENT_IO_START,
    EVENT_IO_DONE,
    EVENT_PREEMPTION,
    EVENT_TERMINATE
} EventType;

typedef enum {
    STATE_NEW,
    STATE_READY,
    STATE_RUNNING,
    STATE_WAITING,
    STATE_TERMINATED
} ProcessState;

typedef enum {
    ALGO_FCFS,
    ALGO_SJF,
    ALGO_RR,
    ALGO_PRIORITY
} Algorithm;

#define CONTEXT_SWITCH_OVERHEAD 1

#endif