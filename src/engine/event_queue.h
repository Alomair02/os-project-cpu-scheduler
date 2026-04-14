#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "../include/types.h"
#include "../include/process.h"

typedef struct {
    int       timestamp;
    EventType type;
    Process  *process;
} Event;

typedef struct {
    Event  *heap;
    int     size;
    int     capacity;
} EventQueue;

EventQueue *eq_create(int capacity);
void        eq_destroy(EventQueue *eq);
void        eq_insert(EventQueue *eq, Event e);
Event       eq_pop(EventQueue *eq);
int         eq_is_empty(EventQueue *eq);

#endif