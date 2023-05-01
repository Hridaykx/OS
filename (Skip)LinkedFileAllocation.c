#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024

// Define a structure to represent a file
typedef struct {
    char name[20];
    int size;
    int start_block;
} file;

// Define a structure to represent a block
typedef struct {
    int next_block;
    char data[BLOCK_SIZE - sizeof(int)];
} block;

// Initialize file system parameters
char disk[NUM_BLOCKS][BLOCK_SIZE];
bool block_used[NUM_BLOCKS];
int num_free_blocks = NUM_BLOCKS;
file files[NUM_BLOCKS];

// Declare file system functions
int allocate_block();
void deallocate_block(int block_index);
void create_file(char* name, int size);
void delete_file(char* name);
void print_filesystem();
void print_files();

int main() {
    // Create some initial files
    create_file("file1", 100);
    create_file("file2", 200);
    create_file("file3", 300);

    // Print file system and file information
    print_filesystem();
    print_files();

    // Delete a file
    delete_file("file2");

    // Create a new file
    create_file("file4", 400);

    // Print file system and file information again
    print_filesystem();
    print_files();

    return 0;
}

// Function to allocate a block
int allocate_block() {
    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (!block_used[i]) {
            block_used[i] = true;
            num_free_blocks--;
            return i;
        }
    }
    return -1; // no free blocks available
}

// Function to deallocate a block
void deallocate_block(int block_index) {
    block_used[block_index] = false;
    num_free_blocks++;
}

// Function to create a file
void create_file(char* name, int size) {
    if (size > num_free_blocks * BLOCK_SIZE) {
        printf("Error: Not enough free space on disk\n");
        return;
    }
    int num_blocks = (size - 1) / BLOCK_SIZE + 1;
    int start_block = allocate_block();
    if (start_block == -1) {
        printf("Error: Not enough free blocks available\n");
        return;
    }
    files[start_block].start_block = start_block;
    strcpy(files[start_block].name, name);
    files[start_block].size = size;
    int prev_block = start_block;
    for (int i = 1; i < num_blocks; i++) {
        int new_block = allocate_block();
        if (new_block == -1) {
            printf("Error: Not enough free blocks available\n");
            deallocate_block(start_block);
            return;
        }
        ((block*)disk[prev_block])->next_block = new_block;
        prev_block = new_block;
    }
    ((block*)disk[prev_block])->next_block = -1; // mark end of file
    memcpy(disk[start_block], name, sizeof(name));
}

// Function to delete a file
void delete_file(char* name) {
    int index = -1;
    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (strcmp(files[i].name, name) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        printf("Error: File not found\n");
        return;
    }
    int block_index = files[index].start_block;
    while (block_index != -1) {

      
