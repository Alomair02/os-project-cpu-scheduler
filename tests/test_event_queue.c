#include <stdio.h>
#include <assert.h>
#include "../src/engine/event_queue.h"

int main() {
    printf("--- Running Event Queue Tests ---\n");

    EventQueue *eq = eq_create(10);

    // 1. Insert events out of chronological order
    Event e1 = {.timestamp = 15, .type = EVENT_CPU_DONE};
    Event e2 = {.timestamp = 2,  .type = EVENT_ARRIVAL};
    Event e3 = {.timestamp = 8,  .type = EVENT_IO_START};
    
    eq_insert(eq, e1);
    eq_insert(eq, e2);
    eq_insert(eq, e3);

    // 2. Pop them and verify they come out in the correct order (2, 8, 15)
    Event popped;
    
    popped = eq_pop(eq);
    assert(popped.timestamp == 2);
    printf("Pop 1 passed (Timestamp: %d)\n", popped.timestamp);

    popped = eq_pop(eq);
    assert(popped.timestamp == 8);
    printf("Pop 2 passed (Timestamp: %d)\n", popped.timestamp);

    popped = eq_pop(eq);
    assert(popped.timestamp == 15);
    printf("Pop 3 passed (Timestamp: %d)\n", popped.timestamp);

    assert(eq_is_empty(eq) == 1);
    printf("Empty check passed\n");

    eq_destroy(eq);
    printf("\nAll Event Queue tests passed! ✅\n");
    return 0;
}