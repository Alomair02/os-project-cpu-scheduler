#ifndef METRICS_H
#define METRICS_H

#include "../engine/simulator.h"
#include "../engine/event_queue.h"

void metrics_record(SimulatorState *s, Event *e);
void metrics_print(SimulatorState *s);

#endif /* METRICS_H */
