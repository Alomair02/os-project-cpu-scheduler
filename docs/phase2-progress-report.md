# Phase 2 Progress Report: CPU Scheduling Simulator

**Course:** ICS 433  
**Submission:** Phase 2 Implementation and Progress  
**Date:** April 25, 2026  
**Repository Branch:** `phase2-submission`

## 1. Project Recap

This project is an event-driven CPU scheduling simulator written in C. Instead of advancing the clock one unit at a time, the simulator processes chronological events such as process arrival, CPU completion, I/O completion, and Round Robin preemption.

The final simulator is intended to accept workload input, run the selected scheduling algorithm, and produce timing metrics plus an ASCII Gantt chart. For Phase 2, the focus is on implementing and validating the core simulation and scheduling modules.

## 2. Implemented So Far

| Area | Status | Notes |
| --- | --- | --- |
| Shared process and event types | Implemented | `Process`, `Event`, `Algorithm`, and process states are defined in shared headers. |
| Event queue | Implemented | Min-heap event queue supports insert, pop, empty check, and dynamic resizing. |
| Simulator engine | Implemented | Seeds arrivals and handles CPU completion, I/O completion, RR preemption, context switches, and process termination. |
| Scheduler algorithms | Implemented | FCFS, SJF, Round Robin, and non-preemptive Priority are implemented behind one scheduler hook interface. |
| Tests | Implemented | Event queue, direct scheduler, and simulator/scheduler integration tests pass. |
| Demo app | Partially implemented | `src/main.c` runs a small manually created workload through the simulator using the real scheduler. |
| I/O parser | Not yet implemented | Placeholder module exists for Phase 3. |
| Metrics summary | Not yet implemented | Temporary event logging exists in the demo; final metrics are Phase 3 work. |
| Output/Gantt chart | Not yet implemented | Placeholder module exists for Phase 3. |

Overall progress is approximately **60-66%**. Two major domains are working: the event-driven engine/event queue and the scheduler layer. The remaining major domain is the user-facing I/O, metrics, and output pipeline.

## 3. Updated System Architecture

    +-------------------+
    | workload input    |  Phase 3
    +---------+---------+
              |
              v
    +-------------------+        +-------------------+
    | Process structs   | -----> | Simulator engine  |
    +-------------------+        +---------+---------+
                                      |     |
                                      |     v
                                      |  Event queue
                                      |
                                      v
                             +-------------------+
                             | Scheduler layer   |
                             | FCFS, SJF, RR,    |
                             | Priority          |
                             +---------+---------+
                                       |
                                       v
                             +-------------------+
                             | metrics + output  |  Phase 3
                             +-------------------+

The simulator engine owns event processing. The scheduler owns only ready-queue ordering and selection. This keeps algorithm decisions separate from event timing and makes it easier to test the scheduler directly.

## 4. Screenshots / Terminal Captures

### Figure 1: Build

    $ make all
    gcc -Wall -Wextra -g -Isrc/include -o scheduler src/main.c src/engine/event_queue.c src/engine/simulator.c src/scheduler/scheduler.c src/scheduler/fcfs.c src/scheduler/sjf.c src/scheduler/rr.c src/scheduler/priority.c src/io/io_handler.c src/metrics/metrics.c src/output/output.c src/output/gantt.c
    gcc -Wall -Wextra -g -Isrc/include -o test_event_queue tests/test_event_queue.c src/engine/event_queue.c
    gcc -Wall -Wextra -g -Isrc/include -o test_scheduler tests/test_scheduler.c src/scheduler/scheduler.c src/scheduler/fcfs.c src/scheduler/sjf.c src/scheduler/rr.c src/scheduler/priority.c
    gcc -Wall -Wextra -g -Isrc/include -o test_simulator tests/test_simulator.c src/engine/event_queue.c src/engine/simulator.c src/scheduler/scheduler.c src/scheduler/fcfs.c src/scheduler/sjf.c src/scheduler/rr.c src/scheduler/priority.c

### Figure 2: Event Queue Test

    $ ./test_event_queue
    --- Running Event Queue Tests ---
    Pop 1 passed (Timestamp: 2)
    Pop 2 passed (Timestamp: 8)
    Pop 3 passed (Timestamp: 15)
    Empty check passed

    All Event Queue tests passed.

### Figure 3: Scheduler Test

    $ ./test_scheduler
    --- Running Scheduler Tests ---
    All scheduler tests passed.

### Figure 4: Simulator + Scheduler Integration Test

    $ ./test_simulator
    --- Running Simulator + Scheduler Tests ---
    Simulator integration tests passed.

### Figure 5: Phase 2 Demo

    $ ./scheduler
    --- Starting Phase 2 CPU Scheduler Demo ---

    Clock:   0 | Event: ARRIVAL      | PID: 1
    Clock:   5 | Event: CPU_DONE     | PID: 1
    Clock:   8 | Event: IO_DONE      | PID: 1
    Clock:  12 | Event: CPU_DONE     | PID: 1

    --- Demo Complete ---

## 5. Challenges Faced

- Keeping module ownership clear while multiple team members worked on separate parts.
- Integrating the scheduler without changing the simulator contract unnecessarily.
- Handling Round Robin preemption in the simulator while keeping ready-queue behavior inside the scheduler.
- Preserving stable tie-breaking for SJF and Priority so tests are deterministic.
- Separating Phase 2 core functionality from Phase 3 CLI, metrics, and output work.

## 6. Plan For Completion

1. Implement workload parsing in `src/io/` and define the accepted workload file format.
2. Replace the demo process in `src/main.c` with command-line selection for scheduling algorithm, workload file, and RR quantum.
3. Implement metrics calculations for turnaround time, waiting time, response time, CPU utilization, and context switch count.
4. Implement final output tables and ASCII Gantt chart rendering in `src/output/`.
5. Add end-to-end tests using workload files for each scheduling algorithm.
6. Run final validation and update documentation with the completed CLI usage.

## 7. Submission Notes

The repository is organized for GitHub submission. The source code is under `src/`, tests are under `tests/`, report files are under `docs/`, and supplemental implementation notes are under `docs/notes/`.

The current Phase 2 code demonstrates the main CPU scheduling modules and test coverage required for partial implementation. The remaining work is clearly scoped and does not block demonstrating the core engine/scheduler behavior.
