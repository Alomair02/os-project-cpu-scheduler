CC     = gcc
CFLAGS = -Wall -Wextra -g -Isrc/include
SRC    = src/main.c \
         src/engine/event_queue.c \
         src/engine/simulator.c \
         src/scheduler/fcfs.c \
         src/scheduler/sjf.c \
         src/scheduler/rr.c \
         src/scheduler/priority.c \
         src/io/io_handler.c \
         src/metrics/metrics.c \
         src/output/output.c \
         src/output/gantt.c
TARGET = scheduler

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)