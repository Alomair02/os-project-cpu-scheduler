CC     = gcc
CFLAGS = -Wall -Wextra -g -Isrc/include
TARGET = scheduler
SCHEDULER_SRC = src/scheduler/scheduler.c \
                src/scheduler/fcfs.c \
                src/scheduler/sjf.c \
                src/scheduler/rr.c \
                src/scheduler/priority.c
ENGINE_SRC = src/engine/event_queue.c \
             src/engine/simulator.c
APP_SRC = src/main.c \
          $(ENGINE_SRC) \
          $(SCHEDULER_SRC) \
          src/io/io_handler.c \
          src/metrics/metrics.c \
          src/output/output.c \
          src/output/gantt.c
TEST_EVENT_QUEUE = test_event_queue
TEST_SCHEDULER = test_scheduler
TEST_SIMULATOR = test_simulator

all: $(TARGET) $(TEST_EVENT_QUEUE) $(TEST_SCHEDULER) $(TEST_SIMULATOR)

$(TARGET): $(APP_SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(APP_SRC)

$(TEST_EVENT_QUEUE): tests/test_event_queue.c src/engine/event_queue.c
	$(CC) $(CFLAGS) -o $(TEST_EVENT_QUEUE) tests/test_event_queue.c src/engine/event_queue.c

$(TEST_SCHEDULER): tests/test_scheduler.c $(SCHEDULER_SRC)
	$(CC) $(CFLAGS) -o $(TEST_SCHEDULER) tests/test_scheduler.c $(SCHEDULER_SRC)

$(TEST_SIMULATOR): tests/test_simulator.c $(ENGINE_SRC) $(SCHEDULER_SRC)
	$(CC) $(CFLAGS) -o $(TEST_SIMULATOR) tests/test_simulator.c $(ENGINE_SRC) $(SCHEDULER_SRC)

test: $(TEST_EVENT_QUEUE) $(TEST_SCHEDULER) $(TEST_SIMULATOR)

clean:
	rm -f $(TARGET) $(TEST_EVENT_QUEUE) $(TEST_SCHEDULER) $(TEST_SIMULATOR)
