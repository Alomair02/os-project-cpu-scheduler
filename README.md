# CPU Scheduling Simulator

Phase 2 implementation for an event-driven CPU scheduling simulator written in C.

## Project Status

The project is about 60-66% complete for Phase 2. The core simulation path is implemented and tested:

- Min-heap event queue for chronological event processing.
- Simulator loop for arrivals, CPU completion, I/O completion, and RR preemption.
- Scheduler layer with FCFS, SJF, Round Robin, and non-preemptive Priority.
- Shared process and event types.
- Unit and integration tests for the event queue, scheduler, and simulator/scheduler interaction.

Remaining Phase 3 work:

- Parse workload files and command-line arguments.
- Compute final metrics such as turnaround time, waiting time, response time, CPU utilization, and context switches.
- Generate final tables and ASCII Gantt chart output.
- Replace the small demo workload in `src/main.c` with the full user-facing CLI flow.

## Repository Layout

```text
src/
  engine/      event queue and simulator loop
  scheduler/   scheduling policies and ready-queue implementation
  include/     shared process, event, and algorithm types
  io/          Phase 3 workload parser placeholder
  metrics/     Phase 3 metrics placeholder
  output/      Phase 3 output placeholder
tests/         module and integration tests
docs/          Phase 2 report and implementation notes
scripts/       helper scripts
```

## Build

```bash
make all
```

This builds:

- `scheduler`
- `test_event_queue`
- `test_scheduler`
- `test_simulator`

## Run Tests

```bash
make test
./test_event_queue
./test_scheduler
./test_simulator
```

Or run everything:

```bash
./scripts/run.sh
```

## Demo

```bash
./scheduler
```

The current demo creates one process manually and runs it through the simulator using FCFS. This proves the Phase 2 engine/scheduler path while workload parsing and final reporting remain scoped for Phase 3.
