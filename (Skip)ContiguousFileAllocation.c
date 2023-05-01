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

// Initialize file system parameters
char disk[NUM_BLOCKS][BLOCK_SIZE];
bool block_used[NUM_BLOCKS];
int num_free_blocks = NUM_BLOCKS;
file files[NUM_BLOCKS];

// Declare file system functions
int allocate_blocks(int num_blocks);
void deallocate_blocks(int start_block, int num_blocks);
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

// Function to allocate contiguous blocks
int allocate_blocks(int num_blocks) {
    int start_block = -1;
    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (block_used[i]) {
            start_block = -1;
            continue;
        }
        if (start_block == -1) {
            start_block = i;
        }
        if (i - start_block + 1 == num_blocks) {
            for (int j = start_block; j <= i; j++) {
                block_used[j] = true;
                num_free_blocks--;
            }
            return start_block;
        }
    }
    return -1; // not enough free blocks available
}

// Function to deallocate contiguous blocks
void deallocate_blocks(int start_block, int num_blocks) {
    for (int i = start_block; i < start_block + num_blocks; i++) {
        block_used[i] = false;
        num_free_blocks++;
    }
}

// Function to create a file
void create_file(char* name, int size) {
    if (size > num_free_blocks * BLOCK_SIZE) {
        printf("Error: Not enough free space on disk\n");
        return;
    }
    int num_blocks = (size - 1) / BLOCK_SIZE + 1;
    int start_block = allocate_blocks(num_blocks);
    if (start_block == -1) {
        printf("Error: Not enough contiguous free blocks available\n");
        return;
    }
    file new_file;
    strcpy(new_file.name, name);
    new_file.size = size;
    new_file.start_block = start_block;
    files[start_block] = new_file;
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
    deallocate_blocks(files[index].start_block, (files[index].size - 1) / BLOCK_SIZE + 1);
    memset(&files[index], 0, sizeof(file));
}

// Function to print
