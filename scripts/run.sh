#!/usr/bin/env sh
set -eu

make clean
make all

./test_event_queue
./test_scheduler
./test_simulator

./scheduler workload.txt fcfs
./scheduler workload.txt sjf
./scheduler workload.txt priority
./scheduler workload.txt rr 4
