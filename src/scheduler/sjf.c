#include "scheduler_internal.h"

static int sjf_comes_before(const ReadyNode *lhs, const ReadyNode *rhs) {
    int lhs_burst = lhs->process->cpu_bursts[lhs->process->current_burst_index];
    int rhs_burst = rhs->process->cpu_bursts[rhs->process->current_burst_index];

    if (lhs_burst != rhs_burst) {
        return lhs_burst < rhs_burst;
    }

    return scheduler_compare_seq_pid(lhs, rhs) < 0;
}

void sjf_enqueue(Process *p) {
    scheduler_insert_ordered(p, sjf_comes_before);
}
