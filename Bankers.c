#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

int num_processes, num_resources;
int available[MAX_RESOURCES];
int max[MAX_PROCESSES][MAX_RESOURCES];
int allocation[MAX_PROCESSES][MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];

bool finish[MAX_PROCESSES];
int safe_sequence[MAX_PROCESSES];
int num_safe_processes = 0;

// Function to check if the system is in a safe state
bool is_safe() {
    // Initialize finish array to false
    for (int i = 0; i < num_processes; i++) {
        finish[i] = false;
    }
    // Initialize work array to available resources
    int work[MAX_RESOURCES];
    for (int i = 0; i < num_resources; i++) {
        work[i] = available[i];
    }
    // Loop through processes until all are finished or unsafe
    bool found_safe_process;
    do {
        found_safe_process = false;
        for (int i = 0; i < num_processes; i++) {
            if (!finish[i]) {
                // Check if process i can be completed
                bool can_be_completed = true;
                for (int j = 0; j < num_resources; j++) {
                    if (need[i][j] > work[j]) {
                        can_be_completed = false;
                        break;
                    }
                }
                if (can_be_completed) {
                    // Add process i to safe sequence and update work array
                    for (int j = 0; j < num_resources; j++) {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    safe_sequence[num_safe_processes++] = i;
                    found_safe_process = true;
                }
            }
        }
    } while (found_safe_process);
    // Check if all processes are finished
    for (int i = 0; i < num_processes; i++) {
        if (!finish[i]) {
            return false;
        }
    }
    return true;
}

int main() {
    // Get input from user
    printf("Enter number of processes: ");
    scanf("%d", &num_processes);
    printf("Enter number of resources: ");
    scanf("%d", &num_resources);
    printf("Enter available resources: ");
    for (int i = 0; i < num_resources; i++) {
        scanf("%d", &available[i]);
    }
    printf("Enter maximum resource allocation for each process: \n");
    for (int i = 0; i < num_processes; i++) {
        printf("Process %d: ", i);
        for (int j = 0; j < num_resources; j++) {
            scanf("%d", &max[i][j]);
        }
    }
    printf("Enter resource allocation for each process: \n");
    for (int i = 0; i < num_processes; i++) {
        printf("Process %d: ", i);
        for (int j = 0; j < num_resources; j++) {
            scanf("%d", &allocation[i][j]);
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
    // Check if the system is in a safe state
    if (is_safe()) {
        // Print the safe sequence
        printf("Safe sequence: ");
        for (int i = 0; i < num_safe_processes; i++) {
            printf("%d ", safe_sequence[i]);
        }
        printf("\n");
    }
