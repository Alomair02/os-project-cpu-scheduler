#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../src/scheduler/scheduler.h"

static Process make_process(int pid, int burst, int priority) {
    Process p;

    memset(&p, 0, sizeof(p));
    p.pid                 = pid;
    p.priority            = priority;
    p.num_cpu_bursts      = 1;
    p.cpu_bursts[0]       = burst;
    p.current_burst_index = 0;
    p.state               = STATE_NEW;
    return p;
}

static SimulatorState make_state(Algorithm algo, int quantum) {
    SimulatorState s;

    memset(&s, 0, sizeof(s));
    s.algo    = algo;
    s.quantum = quantum;
    return s;
}

static void test_fcfs_order(void) {
    SimulatorState s = make_state(ALGO_FCFS, 0);
    Process p1 = make_process(1, 7, 4);
    Process p2 = make_process(2, 3, 2);
    Process p3 = make_process(3, 5, 1);

    sched_init(&s);
    assert(sched_is_empty(&s) == 1);

    sched_enqueue(&s, &p1);
    sched_enqueue(&s, &p2);
    sched_enqueue(&s, &p3);

    assert(sched_next(&s)->pid == 1);
    assert(sched_next(&s)->pid == 2);
    assert(sched_next(&s)->pid == 3);
    assert(sched_is_empty(&s) == 1);
}

static void test_rr_order(void) {
    SimulatorState s = make_state(ALGO_RR, 2);
    Process p1 = make_process(10, 5, 4);
    Process p2 = make_process(11, 5, 4);
    Process p3 = make_process(12, 5, 4);

    sched_init(&s);

    sched_enqueue(&s, &p1);
    sched_enqueue(&s, &p2);
    sched_enqueue(&s, &p3);

    assert(sched_next(&s)->pid == 10);
    sched_enqueue(&s, &p1);
    assert(sched_next(&s)->pid == 11);
    assert(sched_next(&s)->pid == 12);
    assert(sched_next(&s)->pid == 10);
    assert(sched_is_empty(&s) == 1);
}

static void test_sjf_order_and_ties(void) {
    SimulatorState s = make_state(ALGO_SJF, 0);
    Process p1 = make_process(21, 8, 4);
    Process p2 = make_process(22, 3, 2);
    Process p3 = make_process(23, 3, 1);
    Process p4 = make_process(20, 3, 9);

    sched_init(&s);

    sched_enqueue(&s, &p1);
    sched_enqueue(&s, &p2);
    sched_enqueue(&s, &p3);
    sched_enqueue(&s, &p4);

    assert(sched_next(&s)->pid == 22);
    assert(sched_next(&s)->pid == 23);
    assert(sched_next(&s)->pid == 20);
    assert(sched_next(&s)->pid == 21);
}

static void test_priority_order_and_ties(void) {
    SimulatorState s = make_state(ALGO_PRIORITY, 0);
    Process p1 = make_process(31, 8, 4);
    Process p2 = make_process(32, 3, 1);
    Process p3 = make_process(33, 3, 1);
    Process p4 = make_process(30, 3, 7);

    sched_init(&s);

    sched_enqueue(&s, &p1);
    sched_enqueue(&s, &p2);
    sched_enqueue(&s, &p3);
    sched_enqueue(&s, &p4);

    assert(sched_next(&s)->pid == 32);
    assert(sched_next(&s)->pid == 33);
    assert(sched_next(&s)->pid == 31);
    assert(sched_next(&s)->pid == 30);
}

static void test_sched_init_resets_queue(void) {
    SimulatorState s = make_state(ALGO_FCFS, 0);
    Process p1 = make_process(41, 2, 1);

    sched_init(&s);
    sched_enqueue(&s, &p1);
    assert(sched_is_empty(&s) == 0);

    sched_init(&s);
    assert(sched_is_empty(&s) == 1);
}

int main(void) {
    printf("--- Running Scheduler Tests ---\n");

    test_fcfs_order();
    test_rr_order();
    test_sjf_order_and_ties();
    test_priority_order_and_ties();
    test_sched_init_resets_queue();

    printf("All scheduler tests passed.\n");
    return 0;
}
