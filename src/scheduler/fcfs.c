#include "scheduler_internal.h"

void fcfs_enqueue(Process *p) {
    scheduler_push_back(p);
}
