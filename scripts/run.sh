#!/usr/bin/env sh
set -eu

make all

./test_event_queue
./test_scheduler
./test_simulator
./scheduler
