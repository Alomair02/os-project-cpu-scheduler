#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "io_handler.h"

static int skip_line(const char *line)
{
    while (*line && isspace((unsigned char)*line))
        line++;
    return (*line == '\0' || *line == '#');
}

static void print_usage(const char *prog)
{
    fprintf(stderr,
        "Usage: %s <workload_file> <algorithm> [quantum]\n"
        "  algorithm : fcfs | sjf | rr | priority\n"
        "  quantum   : integer > 0, required for rr (default: 4)\n"
        "Example: %s workload.txt rr 4\n",
        prog, prog);
}

int load_workload(const char *filepath, Process ***procs_out)
{
    FILE *fp = fopen(filepath, "r");
    if (!fp) {
        fprintf(stderr, "[io_handler] ERROR: Cannot open workload file '%s'\n",
                filepath);
        return -1;
    }

    Process **procs = malloc(sizeof(Process *) * MAX_PROCESSES);
    if (!procs) {
        fprintf(stderr, "[io_handler] ERROR: Out of memory\n");
        fclose(fp);
        return -1;
    }

    int count = 0;
    char line[512];

    while (fgets(line, sizeof(line), fp)) {
        if (skip_line(line))
            continue;

        if (count >= MAX_PROCESSES) {
            fprintf(stderr,
                "[io_handler] WARNING: Max processes (%d) reached.\n",
                MAX_PROCESSES);
            break;
        }

        int pid, arrival, priority, num_cpu_bursts;
        if (sscanf(line, "%d %d %d %d",
                   &pid, &arrival, &priority, &num_cpu_bursts) != 4) {
            fprintf(stderr,
                "[io_handler] ERROR: Malformed header line: '%s'\n", line);
            continue;
        }

        if (num_cpu_bursts < 1 || num_cpu_bursts > MAX_BURSTS) {
            fprintf(stderr,
                "[io_handler] ERROR: PID %d invalid num_cpu_bursts=%d\n",
                pid, num_cpu_bursts);
            continue;
        }

        char burst_line[512];
        int got_burst_line = 0;
        while (fgets(burst_line, sizeof(burst_line), fp)) {
            if (!skip_line(burst_line)) {
                got_burst_line = 1;
                break;
            }
        }
        if (!got_burst_line) {
            fprintf(stderr,
                "[io_handler] ERROR: PID %d missing burst line\n", pid);
            continue;
        }

        int cpu_bursts[MAX_BURSTS];
        int io_bursts[MAX_BURSTS];
        int num_io_bursts = num_cpu_bursts - 1;
        int total_tokens  = num_cpu_bursts + num_io_bursts;

        char *tok = strtok(burst_line, " \t\n\r");
        int ok = 1;

        for (int t = 0; t < total_tokens; t++) {
            if (!tok) {
                fprintf(stderr,
                    "[io_handler] ERROR: PID %d burst line too short\n", pid);
                ok = 0;
                break;
            }
            int val = atoi(tok);
            if (val <= 0) {
                fprintf(stderr,
                    "[io_handler] ERROR: PID %d burst value must be > 0\n",
                    pid);
                ok = 0;
                break;
            }
            if (t % 2 == 0)
                cpu_bursts[t / 2] = val;
            else
                io_bursts[t / 2]  = val;

            tok = strtok(NULL, " \t\n\r");
        }

        if (!ok)
            continue;

        Process *p = malloc(sizeof(Process));
        if (!p) {
            fprintf(stderr, "[io_handler] ERROR: Out of memory for PID %d\n",
                    pid);
            continue;
        }

        p->pid                 = pid;
        p->arrival_time        = arrival;
        p->priority            = priority;
        p->num_cpu_bursts      = num_cpu_bursts;
        p->num_io_bursts       = num_io_bursts;
        p->current_burst_index = 0;
        p->state               = STATE_NEW;
        p->start_time          = -1;
        p->finish_time         = -1;
        p->waiting_time        = 0;
        p->response_time_set   = 0;

        memcpy(p->cpu_bursts, cpu_bursts, sizeof(int) * num_cpu_bursts);
        memcpy(p->io_bursts,  io_bursts,  sizeof(int) * num_io_bursts);

        procs[count++] = p;
    }

    fclose(fp);

    if (count == 0) {
        fprintf(stderr,
            "[io_handler] ERROR: No processes loaded from '%s'\n", filepath);
        free(procs);
        return -1;
    }

    *procs_out = procs;
    printf("[io_handler] Loaded %d process(es) from '%s'\n", count, filepath);
    return count;
}

int parse_args(int argc, char *argv[],
               char **filepath_out,
               Algorithm *algo_out,
               int *quantum_out)
{
    if (argc < 3) {
        print_usage(argv[0]);
        return -1;
    }

    *filepath_out = argv[1];

    const char *algo_str = argv[2];
    if (strcmp(algo_str, "fcfs") == 0)
        *algo_out = ALGO_FCFS;
    else if (strcmp(algo_str, "sjf") == 0)
        *algo_out = ALGO_SJF;
    else if (strcmp(algo_str, "rr") == 0)
        *algo_out = ALGO_RR;
    else if (strcmp(algo_str, "priority") == 0)
        *algo_out = ALGO_PRIORITY;
    else {
        fprintf(stderr,
            "[io_handler] ERROR: Unknown algorithm '%s'\n", algo_str);
        print_usage(argv[0]);
        return -1;
    }

    *quantum_out = 4;
    if (*algo_out == ALGO_RR) {
        if (argc >= 4) {
            *quantum_out = atoi(argv[3]);
            if (*quantum_out <= 0) {
                fprintf(stderr,
                    "[io_handler] ERROR: Quantum must be > 0\n");
                return -1;
            }
        } else {
            printf("[io_handler] No quantum provided for RR, defaulting to 4\n");
        }
    }

    return 0;
}
