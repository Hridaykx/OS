#include <stdio.h>
#include <stdlib.h>

#define CACHE_SIZE 10

// Define a structure to represent a cache block
typedef struct {
    int data;
    int last_used;
} cache_block;

// Initialize cache contents
cache_block cache[CACHE_SIZE];

// Declare global hit and miss counts
int hit_count = 0;
int miss_count = 0;

// Function to check if an item is in the cache
int is_item_in_cache(int item) {
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (cache[i].data == item) {
            hit_count++;
            return 1; // item found in cache
        }
    }
    miss_count++;
    return 0; // item not found in cache
}

int main() {
    // Initialize cache contents with random data
    for (int i = 0; i < CACHE_SIZE; i++) {
        cache[i].data = rand() % 100;
        cache[i].last_used = -1; // initialize last used time to -1
    }

    // Define a sequence of memory requests
    int requests[] = {2, 5, 7, 1, 2, 8, 5, 6, 9, 2, 5, 1, 4, 0};

    // Process memory requests using Optimal policy
    for (int i = 0; i < sizeof(requests) / sizeof(int); i++) {
        int item = requests[i];
        if (!is_item_in_cache(item)) {
            // If item is not in cache, find the item that will not be used for the longest time in the future
            int max_future_index = 0;
            int max_future_time = -1;
            for (int j = 0; j < CACHE_SIZE; j++) {
                int future_time = -1;
                for (int k = i + 1; k < sizeof(requests) / sizeof(int); k++) {
                    if (requests[k] == cache[j].data) {
                        future_time = k;
                        break;
                    }
                }
                if (future_time == -1) {
                    max_future_index = j;
                    break;
                }
                if (future_time > max_future_time) {
                    max_future_index = j;
                    max_future_time = future_time;
                }
            }
            // Replace the cache block with the item that will not be used for the longest time in the future
            cache[max_future_index].data = item;
            cache[max_future_index].last_used = i;
        }
    }

    // Calculate hit ratio and miss ratio
    float hit_ratio = (float) hit_count / (float) sizeof(requests) * sizeof(int);
    float miss_ratio = (float) miss_count / (float) sizeof(requests) * sizeof(int);

    // Print results
    printf("Hit count: %d\n", hit_count);
    printf("Miss count: %d\n", miss_count);
    printf("Hit ratio: %.2f%%\n", hit_ratio * 100);
    printf("Miss ratio: %.2f%%\n", miss_ratio * 100);

    return 0;
}
