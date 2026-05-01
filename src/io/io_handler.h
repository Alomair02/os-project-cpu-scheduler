#ifndef IO_HANDLER_H
#define IO_HANDLER_H

#include "../include/process.h"
#include "../include/types.h"

#define MAX_PROCESSES 256

int load_workload(const char *filepath, Process ***procs_out);

int parse_args(int argc, char *argv[],
               char **filepath_out,
               Algorithm *algo_out,
               int *quantum_out);

#endif /* IO_HANDLER_H */
