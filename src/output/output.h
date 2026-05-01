#ifndef OUTPUT_H
#define OUTPUT_H

#include "../engine/simulator.h"
#include "gantt.h"

void print_results(SimulatorState *s, GanttChart *chart);
void print_header(SimulatorState *s);

#endif /* OUTPUT_H */
