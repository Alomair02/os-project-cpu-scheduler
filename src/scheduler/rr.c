#include "scheduler_internal.h"

void rr_enqueue(Process *p) {
    scheduler_push_back(p);
}
