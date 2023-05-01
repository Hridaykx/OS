#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024
#define NUM_FILES 128

// Define a structure to represent a file
typedef struct {
    char name[20];
    int size;
    int index_block;
} file;

// Initialize file system parameters
char disk[NUM_BLOCKS][BLOCK_SIZE];
bool block_used[NUM_BLOCKS];
int num_free_blocks = NUM_BLOCKS - 1;
file files[NUM_FILES];
int num_files = 0;

// Declare file system functions
int allocate_block();
void deallocate_block(int block);
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

// Function to allocate a free block
int allocate_block() {
    for (int i = 1; i < NUM_BLOCKS; i++) {
        if (!block_used[i]) {
            block_used[i] = true;
            num_free_blocks--;
            return i;
        }
    }
    return -1; // no free blocks available
}

// Function to deallocate a block
void deallocate_block(int block) {
    block_used[block] = false;
    num_free_blocks++;
}

// Function to create a file
void create_file(char* name, int size) {
    if (num_files == NUM_FILES) {
        printf("Error: Maximum number of files reached\n");
        return;
    }
    if (size > num_free_blocks * BLOCK_SIZE) {
        printf("Error: Not enough free space on disk\n");
        return;
    }
    file new_file;
    strcpy(new_file.name, name);
    new_file.size = size;
    new_file.index_block = allocate_block();
    if (new_file.index_block == -1) {
        printf("Error: No free blocks available\n");
        return;
    }
    int num_blocks = (size - 1) / BLOCK_SIZE + 1;
    int* data_blocks = malloc(num_blocks * sizeof(int));
    for (int i = 0; i < num_blocks; i++) {
        data_blocks[i] = allocate_block();
        if (data_blocks[i] == -1) {
            printf("Error: No free blocks available\n");
            for (int j = 0; j < i; j++) {
                deallocate_block(data_blocks[j]);
            }
            deallocate_block(new_file.index_block);
            free(data_blocks);
            return;
        }
    }
    memcpy(disk[new_file.index_block], data_blocks, num_blocks * sizeof(int));
    free(data_blocks);
    files[num_files] = new_file;
    num_files++;
}

// Function to delete a file
void delete_file(char* name) {
    int index = -1;
    for (int i = 0; i < num_files; i++) {
        if (strcmp(files[i].name, name) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        printf("Error: File not found\n");
        return
