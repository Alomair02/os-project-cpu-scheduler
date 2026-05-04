# CPU Scheduling Simulator

Final C implementation of an event-driven CPU scheduling simulator for ICS 433.
The simulator reads a workload file, runs one scheduling algorithm, and prints a
Gantt chart plus per-process and system-wide metrics.

## Requirements

- Linux, macOS, or WSL
- `gcc`
- `make`

No third-party libraries are required.

## Build

```bash
make clean
make all
```

This builds:

- `scheduler`
- `test_event_queue`
- `test_scheduler`
- `test_simulator`

## Run

```bash
./scheduler <workload_file> <algorithm> [quantum]
```

Supported algorithms:

- `fcfs` - First-Come First-Served
- `sjf` - non-preemptive Shortest Job First
- `rr` - Round Robin
- `priority` - non-preemptive Priority Scheduling, where a lower numeric value means higher priority

Examples:

```bash
./scheduler workload.txt fcfs
./scheduler workload.txt sjf
./scheduler workload.txt priority
./scheduler workload.txt rr 4
```

If Round Robin is selected and no quantum is provided, the simulator defaults to
quantum `4`.

## Workload Format

Each process uses two logical lines:

```text
pid arrival_time priority num_cpu_bursts
cpu_1 io_1 cpu_2 io_2 ... cpu_n
```

Rules:

- Blank lines are ignored.
- Lines starting with `#` are ignored.
- `num_cpu_bursts` must be at least `1`.
- Number of I/O bursts is `num_cpu_bursts - 1`.
- CPU and I/O burst values must be positive integers.

The repository includes `workload.txt` as a sample input file.

## Tests

```bash
make test
./test_event_queue
./test_scheduler
./test_simulator
```

`make test` builds the test binaries. Run the binaries to execute the assertions.

The helper script runs build, tests, and all sample algorithms:

```bash
./scripts/run.sh
```

## Repository Structure

```text
src/
  engine/      event queue and simulation loop
  scheduler/   FCFS, SJF, RR, and Priority ready-queue policies
  io/          workload parsing and command-line parsing
  metrics/     response, waiting, turnaround, utilization, throughput
  output/      final report tables and ASCII Gantt chart
  include/     shared process and enum definitions
tests/         event queue, scheduler, and simulator integration tests
docs/          final report and supplemental notes
scripts/       helper scripts
```

## Team

| Name | ID | Main Contribution |
| --- | --- | --- |
| Abdulaziz Alomair | 202185570 | Simulation engine and event queue |
| Feras Alharbi | 202258300 | Scheduling algorithms |
| Faisal Alyousef | 202171870 | I/O, metrics, output, Gantt chart, and main integration |
