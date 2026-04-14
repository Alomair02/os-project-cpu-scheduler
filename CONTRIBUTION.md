# CPU Scheduling Simulator

## Project Overview
The goal of this project is to design and implement an event-driven CPU Scheduling Simulator in C. Instead of advancing time step-by-step, the simulator progresses based on discrete system events like process arrivals and I/O completions. 

## Team Roles & Responsibilities

To keep our work modular and avoid stepping on each other's toes, we have divided the project into three distinct domains:

### 1. Abdulaziz Alomair (Simulation Engine & Event Queue)
* **Role:** Building the core heart of the simulator.
* **Responsibilities:** * Implementing the min-heap Event Queue.
    * Writing the main event loop that processes arrivals, CPU/IO completions, and preemptions.
    * Defining the shared contract files (`types.h`, `process.h`, `simulator.h`).

### 2. Feras Alharbi (Scheduling Algorithms)
* **Role:** Routing processes based on policy.
* **Responsibilities:**
    * Implementing the core algorithms: FCFS, SJF, Round Robin, and Priority.
    * Managing the algorithm-specific Ready Queues (e.g., FIFO for FCFS, sorted for SJF/Priority, circular for RR).
    * Providing the hooks that the Engine will call (`sched_enqueue`, `sched_next`).

### 3. Faisal Alyousef (I/O, Metrics & Output)
* **Role:** Handling the program's edges (Input, Math, and Output).
* **Responsibilities:**
    * **Input:** Parsing the `workload.txt` file and command-line arguments to initialize the processes.
    * **Metrics:** Tracking wait times, turnaround times, CPU utilization, and context switches.
    * **Output:** Generating the final ASCII Gantt chart and statistics tables.

---

## Directory Structure & Ownership

Please only edit files within your assigned directories unless agreed upon by the team.

```text
project/
├── Makefile                 # Shared
├── workload.txt             # Faisal (Format definition)
├── src/
│   ├── include/             # Shared (Contract headers: types.h, process.h)
│   ├── main.c               # Faisal (CLI parsing & setup)
│   ├── engine/              # Abdulaziz (event_queue.c, simulator.c)
│   ├── scheduler/           # Feras (fcfs.c, sjf.c, rr.c, priority.c)
│   ├── io/                  # Faisal (io_handler.c)
│   ├── metrics/             # Faisal (metrics.c)
│   └── output/              # Faisal (output.c, gantt.c)
├── tests/                   # Shared (Write tests for your own modules here)
└── scripts/                 # Shared (Helper scripts like run.sh)
```

---

## Git Routine: How We Collaborate

To prevent overwriting each other's code, we will use a **Branch & Pull Request** workflow. **Never commit directly to the `main` branch.**

### Step 1: Get the latest code
Before you start working for the day, make sure your local `main` branch is up to date:
```bash
git checkout main
git pull origin main
```

### Step 2: Create a branch for your work
Always create a new branch for the specific feature you are building. Name it clearly:
```bash
# Example: git checkout -b abdulaziz-event-queue
git checkout -b <your-name>-<feature-name>
```

### Step 3: Write code, Stage, and Commit
As you build and test your code, save your changes to your branch:
```bash
# See what files you changed
git status

# Stage the files you want to save
git add .

# Commit with a clear, descriptive message
git commit -m "Add min-heap implementation for event queue"
```

### Step 4: Push your branch to GitHub
When your feature is working and you are ready to share it:
```bash
git push origin <your-name>-<feature-name>
```

### Step 5: Open a Pull Request (PR)
1. Go to the repository on GitHub.
2. You will see a green button that says **Compare & pull request** next to your recently pushed branch. Click it.
3. Add a brief description of what you built.
4. Click **Create pull request**.
5. Let the team know in chat. Someone else should ideally glance at the code and click **Merge pull request** to bring your code into the `main` branch. 

Once merged, everyone repeats Step 1 to pull your new code into their computers!