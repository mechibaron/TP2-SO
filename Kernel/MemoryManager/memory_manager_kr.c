#ifndef BUDDY_MM

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <memoryManager.h>

#define TRUE 1
#define FALSE !TRUE
// #define MEMORY_MANAGER_NAME "Size allocation"
// #define START_ADDRESS 0xF00000


typedef long Align;

typedef union header {
    struct {
        union header* ptr;
        uint64_t size;
    } data;

    Align x;
} header;

static header* base;
static header* free_node = NULL;

uint64_t total_heap_size;
char* start_address;

void createMemory(char *heap_base, size_t heap_size) {
    if (heap_size == 0) {
        return;
    }
    // char* start_address = (char*)START_ADDRESS;

    total_heap_size = (heap_size + sizeof(header) - 1) / sizeof(header) + 1;
    free_node = base = (header*)heap_base;
    free_node->data.size = total_heap_size;
    free_node->data.ptr = free_node;
}

void* memory_manager_malloc(size_t nbytes) {
    if (nbytes == 0) {
        return NULL;
    }

    void* result;
    header* current_node, * previous_ptr;
    uint64_t malloc_units = (nbytes + sizeof(header) - 1) / sizeof(header) + 1;
    previous_ptr = free_node;
    char node_found;
    node_found = TRUE;

    for (current_node = previous_ptr->data.ptr; node_found; current_node = current_node->data.ptr) {
        if (current_node->data.size >= malloc_units) {
            if (current_node->data.size == malloc_units) {
                previous_ptr->data.ptr = current_node->data.ptr;
            }
            else {
                current_node->data.size -= malloc_units;
                current_node += current_node->data.size;
                current_node->data.size = malloc_units;
            }
            free_node = previous_ptr;
            result = (void*)(current_node + 1);
            node_found = FALSE;
        }
        if (current_node == free_node) {
            return NULL;
        }
        previous_ptr = current_node;
    }
    return result;
}

void free_memory_manager(void* block_blocked) {
    if (block_blocked == NULL) {
        return;
    }

    header *free_block, *current_node;
    free_block = (header*)block_blocked -1;
    // header* free_block = (header*)((char*)block_blocked - sizeof(header));
    if (free_block < base || free_block >= (base + total_heap_size * sizeof(header))) {
        return;
    }

    block_blocked = NULL;

    // header* current_node;
    char check_external = TRUE;

    for (current_node = free_node; !(free_block > current_node && free_block < current_node->data.ptr) && !check_external; current_node = current_node->data.ptr) {
        if (free_block == current_node || free_block == current_node->data.ptr) {
            return;
        }
        if (current_node >= current_node->data.ptr && (free_block > current_node || free_block < current_node->data.ptr)) {
            check_external = TRUE;
        }
         if (!check_external && (current_node + current_node->data.size > free_block || free_block + free_block->data.size > current_node->data.ptr)) {
        return;
    }

   if (free_block + free_block->data.size == current_node->data.ptr) {
        free_block->data.size += current_node->data.ptr->data.size;
        free_block->data.ptr = current_node->data.ptr->data.ptr;
    } else {
        free_block->data.ptr = current_node->data.ptr;
    }

    if (current_node + current_node->data.size == free_block) {
        current_node->data.size += free_block->data.size;
        current_node->data.ptr = free_block->data.ptr;
    } else {
        current_node->data.ptr = free_block;
    }

    free_node = current_node;
}
}

void *memory_dump(){
    int block_number = 1;
    header* first;
    header* current;
    first = current = free_node;

    char flag = TRUE;
    printf("\nUtilizando KR_MEMORY_MANAGER\n");
    printf("VUELCO DE MEMORIA \n");
    printf("\nMemoria Total: %d bytes\n\n", (uint32_t)total_heap_size * sizeof(header));

    if (free_node == NULL) {
        printf("\nNo hay bloques de memoria disponibles.\n");
        return;
    }
    printf("Bloques libres:\n\n");

    while (current != first || flag) {
        flag = FALSE;
        printf("\tBloque numero: %d\n", block_number);
        printf("\tBase:%x\n", (uint64_t)current);
        printf("\tBytes disponibles: %d\n\n", (int)current->data.size);

        current = current->data.ptr;
        block_number++;
    }

    printf("\n");
}
// MemoryInfo* mem_info() {
//     MemoryInfo* info = (MemoryInfo*)memory_manager_malloc(sizeof(MemoryInfo));
//     if (info != NULL) {
//         info->memoryAlgorithmName = MEMORY_MANAGER_NAME;
//         info->totalMemory = total_heap_size * sizeof(header);
//         info->freeMemory = 0;
//         info->occupiedMemory = 0;
//         info->blocksUsed = 0;

//         size_t free_mem = 0;
//         int block_number = 0;
//         header* current = free_node;
//         char flag = 1;

//         while (current != free_node || flag) {
//             flag = 0;
//             free_mem += current->data.size * sizeof(header);
//             block_number++;
//             current = current->data.ptr;
//         }

//         info->freeMemory = free_mem;
//         info->occupiedMemory = info->totalMemory - free_mem;
//         info->blocksUsed = block_number;
//     }

//     return info;
// }

#endif
