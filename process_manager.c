#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#define MAX_BAR_LENGTH 50 

// ANSI color codes
#define COLOR_RESET "\033[0m"
#define COLOR_PURPLE "\033[35m"
#define COLOR_GREEN "\033[32m"
#define COLOR_RED "\033[31m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_CYAN "\033[36m"
#define COLOR_WHITE "\033[37m"

// Function to clear the screen
void clear_screen() {
    system("clear"); 
}

// Function to set the cursor position using ANSI escape codes
void setCursorPosition(int x, int y) {
    printf("\033[%d;%dH", y, x); 
}

// Function to check if a key has been pressed
int kbhit() {
    struct termios oldt, newt;
    int oldf;
    char ch;
    int temp;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    temp = read(STDIN_FILENO, &ch, 1);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(temp == 1) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

// Function to get a user-friendly description of the process status
const char* get_status_description(char state) {
    switch (state) {
        case 'R': return COLOR_GREEN "Running" COLOR_RESET;
        case 'S': return COLOR_YELLOW "Sleeping" COLOR_RESET;
        case 'D': return COLOR_RED "Waiting" COLOR_RESET;
        case 'T': return COLOR_CYAN "Stopped" COLOR_RESET;
        case 'Z': return COLOR_RED "Zombie" COLOR_RESET;
        case 'I': return COLOR_BLUE "Idle" COLOR_RESET;
        case 'X': return COLOR_RED "Dead" COLOR_RESET;
        case 'P': return COLOR_WHITE "Parked" COLOR_RESET;
        default: return COLOR_WHITE "Unknown" COLOR_RESET;
    }
}

// Function to display a simple bar graph based on CPU usage
void display_bar_graph(int cpu_usage) {
    int bar_length = (cpu_usage * MAX_BAR_LENGTH) / 100; // Scale CPU usage to bar length
    printf(" [");
    for (int i = 0; i < MAX_BAR_LENGTH; i++) {
        if (i < bar_length) {
            printf(COLOR_GREEN "#"); // Filled portion in green
        } else {
            printf(" "); // Empty portion
        }
    }
    printf(COLOR_WHITE "] %d%%", cpu_usage);
    
}

// Function to convert bytes to megabytes
double bytes_to_megabytes(unsigned long bytes) {
    return bytes / (1024.0 * 1024.0);
}

// Function to list processes with additional attributes
void list_processes(int show_all) {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir("/proc")) == NULL) {
        perror("opendir");
        return;
    }

    setCursorPosition(0, 5); // Set cursor position to the top
    printf("%-10s %-35s %-15s %-60s %-10s %-10s %-10s\n", "PID", "Process Name", "Status", "CPU Usage", "RSS (MB)", "VSZ (MB)", "Threads");
    printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    int line = 8; 

    while ((entry = readdir(dir)) != NULL) {
        if (isdigit(*entry->d_name)) {
            char path[256];
            sprintf(path, "/proc/%s/stat", entry->d_name);
            FILE *fp = fopen(path, "r");
            if (fp) {
                char comm[256]; 
                char state; 
                unsigned long utime, stime, rss, vsize;
                int num_threads, pid;

                if (fscanf(fp, "%d %s %c %*d %*d %*d %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %*u %lu %lu %*u %*u %lu %lu %d",
                           &pid, comm, &state, &utime, &stime, &rss, &vsize, &num_threads) != 8) {
                    fclose(fp);
                    continue; 
                }

                fclose(fp);

                comm[strlen(comm) - 1] = '\0'; 
                memmove(comm, comm + 1, strlen(comm)); 

                const char* status_desc = get_status_description(state);
                int cpu_usage = (utime + stime) % 100; 

                if (show_all || state != 'S') { 
                    setCursorPosition(0, line);
                    printf(COLOR_PURPLE "%-10d" COLOR_RESET " %-35s %-23s ", pid, comm, status_desc);
                    display_bar_graph(cpu_usage);
                    printf(COLOR_RESET"      %-10.2f %-10.2f %-10d\n", bytes_to_megabytes(rss), bytes_to_megabytes(vsize), num_threads);
                    line++; 
                }
            }
        }
    }

    closedir(dir);
}

int main() {
    int show_all = 0; 

    while (1) {
        clear_screen();

        printf("Process Manager\n=================\n");
        printf("Press 'q' to exit, 'a' to show all processes.\n");
        list_processes(show_all);
        sleep(1); 

        if (kbhit()) {
            char c = getchar();
            if (c == 'q') {
                break; 
            } else if (c == 'a') {
                show_all = !show_all; 
            }
        }
    }

    return 0;
}
