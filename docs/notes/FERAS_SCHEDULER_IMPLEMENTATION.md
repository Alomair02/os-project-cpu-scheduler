# Feras Scheduler Implementation Log

## Goal And Scope
- Implement the Phase 1 scheduling layer for FCFS, SJF, RR, and non-preemptive Priority.
- Stay inside scheduler-owned files plus shared build/tests and documentation.
- Avoid editing teammate-owned runtime modules such as `src/main.c`, `src/engine/`, `src/io/`, `src/metrics/`, and `src/output/`.

## Files Touched
- `src/scheduler/scheduler_internal.h`
- `src/scheduler/scheduler.c`
- `src/scheduler/fcfs.c`
- `src/scheduler/sjf.c`
- `src/scheduler/rr.c`
- `src/scheduler/priority.c`
- `src/scheduler/scheduler.h`
- `tests/test_scheduler.c`
- `tests/test_simulator.c`
- `Makefile`
- `.gitignore`
- `FERAS_SCHEDULER_IMPLEMENTATION.md`

## Scheduler API Contract
- `sched_init(SimulatorState *s)`
- `sched_enqueue(SimulatorState *s, Process *p)`
- `sched_next(SimulatorState *s)`
- `sched_is_empty(SimulatorState *s)`

These remain aligned with the engine hook contract already used by `src/engine/simulator.c`.

## Design Decisions
- Implemented a single internal ready queue as a singly linked list.
- Added `ready_seq` to queue nodes, not to `Process`, to avoid unnecessary shared-struct changes.
- Stable ordering rules:
  - FCFS: enqueue order
  - RR: enqueue order
  - SJF: next CPU burst, then enqueue order, then PID
  - Priority: lower numeric priority, then enqueue order, then PID
- `Priority` is implemented as non-preemptive for Phase 1.
- RR quantum validation is enforced during `sched_init`; invalid RR quantum aborts with an error message.
- Scheduler state is internal and static because the current simulator is effectively single-instance.

## Implementation Log
1. Confirmed the engine expects only four scheduler hooks and already performs RR preemption and remaining-burst reduction.
2. Confirmed the project PDF requires FCFS, SJF, RR, and Priority, with SJF and FCFS non-preemptive.
3. Chose a shared linked-list ready queue to minimize complexity and keep algorithm files focused on ordering policy.
4. Added `scheduler_internal.h` to isolate queue internals from the engine-facing header.
5. Implemented `scheduler.c` with queue lifecycle, ordered insertion, dispatch routing, and RR quantum validation.
6. Implemented per-algorithm enqueue files:
   - `fcfs.c`
   - `sjf.c`
   - `rr.c`
   - `priority.c`
7. Added `tests/test_scheduler.c` for direct ready-queue ordering and reset behavior.
8. Replaced the mock-based `tests/test_simulator.c` with real scheduler integration coverage for SJF, RR, Priority, and I/O re-entry.
9. Updated `Makefile` so the real scheduler is compiled by dedicated test targets without touching `src/main.c`.
10. Updated `.gitignore` so the repo tracks `src/scheduler/` and `tests/test_scheduler.c` instead of accidentally ignoring them.
11. Preserved the existing engine contract to avoid shared-header churn.

## Integration Notes
- `src/main.c` still contains mock `sched_*` definitions owned outside my scope.
- Because of that, the real scheduler module is compiled through dedicated scheduler/test targets in `Makefile` instead of being linked into the current harness target.
- Once `src/main.c` stops exporting mock scheduler hooks, the real scheduler sources can be linked into the main application target directly.

## Test Coverage Added
- Direct scheduler ordering tests for FCFS, RR, SJF, and Priority.
- Queue reset and empty-state behavior.
- Simulator integration tests for:
  - SJF non-preemption
  - RR round-robin rotation
  - Priority non-preemption
  - I/O re-entry through the scheduler

## Known Constraints
- This environment does not currently expose `gcc` or `make`, so I cannot run the new targets here.
- Final verification should be performed in the team's intended Linux/gcc environment.
