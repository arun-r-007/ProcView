# 🧠  ProcView – A Real-Time Linux Terminal-Based Process Viewer in C

A colorful, real-time, terminal-based process manager written in **C** for Linux systems. This project mimics a simplified version of tools like `top` or `htop`, displaying critical process information directly from the `/proc` filesystem.

---

## 📁 Project Structure

```
process_manager/
├── process_manager.c      # Main source file (your code)
├── README.md              # This file
```

---

## 🚀 Features

* 🧵 **Displays Active Processes** with:

  * PID (Process ID)
  * Process Name
  * Process Status (Color-coded)
  * CPU Usage (Animated Bar Graph)
  * RSS (Resident Set Size) in MB
  * VSZ (Virtual Memory Size) in MB
  * Number of Threads

* 🎨 **Colorful ANSI Output** for easier readability.

* 📉 **Real-time Bar Graph** showing CPU usage visually.

* 🔄 **Live Refreshing UI** – updates every second.

* 🎛️ **Keyboard Interactions**:

  * Press `'a'` to toggle **show all processes** including sleeping ones.
  * Press `'q'` to **exit** the program.

---

## ⚙️ How It Works

* It reads from the `/proc` pseudo-filesystem.
* Parses `/proc/[PID]/stat` to gather:

  * CPU usage (utime + stime)
  * Process state
  * Memory usage (RSS and VSZ)
  * Number of threads
* Uses ANSI escape codes to:

  * Clear screen
  * Move the cursor
  * Color-code the output
* Custom `kbhit()` implementation to detect keypresses without blocking the display loop.

---

## 🛠️ Compilation & Run

### 🔧 Requirements

* Linux OS
* GCC compiler

### ⚙️ Compile

```bash
gcc process_manager.c -o process_manager
```

### ▶️ Run

```bash
./process_manager
```

> 📝 Run in a terminal window with enough width for the best display (ideally 120+ columns).

---

## 📸 Sample Output

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

---

## 🎯 Objectives

* Understand **/proc filesystem** in Linux.
* Practice **low-level process inspection** and **file parsing** in C.
* Apply **real-time terminal UI** concepts using ANSI escape sequences.
* Use **non-blocking input** (`kbhit`) to enable user interaction without stopping the display loop.

---

## 🧪 Testing Notes

* Tested on: **Ubuntu 22.04 LTS**, **WSL**, and **VirtualBox-based Linux**
* May require elevated permissions to access certain process details.
* Works best in terminal emulators that support ANSI codes.

---

## 📚 Future Enhancements (Optional Ideas)

* Add memory usage graph (similar to CPU bar).
* Include parent PID, user info (`/proc/[pid]/status`).
* Sort by CPU or memory usage.
* Export process list to file.
