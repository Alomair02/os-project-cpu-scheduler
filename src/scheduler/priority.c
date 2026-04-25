#include "scheduler_internal.h"

static int priority_comes_before(const ReadyNode *lhs, const ReadyNode *rhs) {
    if (lhs->process->priority != rhs->process->priority) {
        return lhs->process->priority < rhs->process->priority;
    }

    return scheduler_compare_seq_pid(lhs, rhs) < 0;
}

void priority_enqueue(Process *p) {
    scheduler_insert_ordered(p, priority_comes_before);
}
