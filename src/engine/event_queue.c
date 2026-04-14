#include <stdlib.h>
#include <stdio.h>
#include "event_queue.h"

EventQueue *eq_create(int capacity) {
    EventQueue *eq = malloc(sizeof(EventQueue));
    eq->heap     = malloc(sizeof(Event) * capacity);
    eq->size     = 0;
    eq->capacity = capacity;
    return eq;
}

void eq_destroy(EventQueue *eq) {
    free(eq->heap);
    free(eq);
}

static void swap(Event *a, Event *b) {
    Event tmp = *a; *a = *b; *b = tmp;
}

static void heapify_up(EventQueue *eq, int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (eq->heap[parent].timestamp <= eq->heap[i].timestamp) break;
        swap(&eq->heap[parent], &eq->heap[i]);
        i = parent;
    }
}

static void heapify_down(EventQueue *eq, int i) {
    int n = eq->size;
    while (1) {
        int smallest = i;
        int l = 2 * i + 1, r = 2 * i + 2;
        if (l < n && eq->heap[l].timestamp < eq->heap[smallest].timestamp) smallest = l;
        if (r < n && eq->heap[r].timestamp < eq->heap[smallest].timestamp) smallest = r;
        if (smallest == i) break;
        swap(&eq->heap[i], &eq->heap[smallest]);
        i = smallest;
    }
}

void eq_insert(EventQueue *eq, Event e) {
    if (eq->size >= eq->capacity) {
        eq->capacity *= 2;
        eq->heap = realloc(eq->heap, sizeof(Event) * eq->capacity);
    }
    eq->heap[eq->size++] = e;
    heapify_up(eq, eq->size - 1);
}

Event eq_pop(EventQueue *eq) {
    if (eq->size == 0) {
        fprintf(stderr, "eq_pop: queue is empty\n");
        exit(EXIT_FAILURE);
    }
    Event top = eq->heap[0];
    eq->heap[0] = eq->heap[--eq->size];
    heapify_down(eq, 0);
    return top;
}

int eq_is_empty(EventQueue *eq) {
    return eq->size == 0;
}