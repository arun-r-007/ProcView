# ProcView

ProcView is a real-time terminal-based process viewer written in C for Linux. It reads data directly from the `/proc` filesystem and displays a live, simplified process monitor interface.
Inspired by classic tools like `top` and `htop`, ProcView provides a color-coded, minimal UI focused on essential process metrics.

---

## What It Does

- Displays a live table of processes every second.
- Fetches data from `/proc/[PID]/stat` and `/proc/[PID]/status`.
- Shows:
  - **PID** – Process ID
  - **Process Name** – Executable name
  - **Status** – Current state of the process (color-coded)
  - **CPU Usage** – Animated bar graph showing process CPU load
  - **RSS** – Resident Set Size (memory used in RAM)
  - **VSZ** – Virtual Memory Size (total virtual memory used)
  - **Threads** – Number of threads used by the process

---

## Keyboard Inputs

- Press `q` – Quit the program  
- Press `a` – Toggle between showing only running processes and all processes (including sleeping)

---

## Colors and Status Codes

| Color     | Status Code | Meaning                                 |
|-----------|-------------|------------------------------------------|
| Green     | R           | Running – actively using CPU             |
| Yellow    | S           | Sleeping – idle, waiting for input       |
| Cyan      | D           | Uninterruptible Sleep – waiting on I/O   |
| Red       | Z           | Zombie – terminated but not cleaned up   |
| Magenta   | T           | Stopped – process stopped by a signal    |

---

## Performance Meaning of Metrics

- **CPU Usage**  
  Indicates how much processing time the process is using. High usage implies active computation. The bar graph shows relative usage visually.

- **RSS (Resident Set Size)**  
  The actual physical RAM used. Useful to gauge memory pressure.

- **VSZ (Virtual Size)**  
  The total virtual memory allocated, including code, stack, and memory-mapped files. A high VSZ with low RSS means memory is reserved but not necessarily used.

- **Threads**  
  Indicates how many threads the process is using. High thread counts often suggest multitasking or concurrency.

---

## Sample Output

```
Process Manager
=================
Press 'q' to exit, 'a' to show all processes.
PID        Process Name                       Status                  CPU Usage                                                      RSS (MB)   VSZ (MB)   Threads
-----------------------------------------------------------------------------------------------------------------------------------------------------------
1024       bash                               Sleeping                [#########################                         ] 43%      3.48       120.55      1
1456       code                               Running                 [#########################################         ] 78%      254.63     1200.48     25
...
```


## Testing Notes

* Tested on: **Ubuntu 22.04 LTS**, **WSL**, and **VirtualBox-based Linux**
* May require elevated permissions to access certain process details.
* Works best in terminal emulators that support ANSI codes.

