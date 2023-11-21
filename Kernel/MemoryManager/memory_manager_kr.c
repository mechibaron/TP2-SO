#ifndef BUDDY_MM

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <memoryManager.h>

#define TRUE 1
#define FALSE !TRUE
#define START_ADDRESS 0xF00000
#define MEMORY_MANAGER_NAME "Dynamic memory allocation"

size_t totalHeapMemory;
size_t usedHeapMemory;
size_t memoryBlockCount;

BlockADT head = NULL;


// static header* base;
// static header* free_node = NULL;
// uint64_t total_heap_size;

// static header* base;

// uint64_t total_heap_size;
// char* start_address;

void createMemory(size_t size) {
    totalHeapMemory = size;
    usedHeapMemory = sizeof(BlockCDT);
    memoryBlockCount = 1;
    head = (BlockADT)START_ADDRESS;
    head->size = 0;
    head->unused = totalHeapMemory - usedHeapMemory;
    
    head->prev = NULL;
    head->next = NULL;
}

void* memory_manager_malloc(size_t nbytes) {
if (nbytes == 0)
        return NULL;

    BlockADT current = head;
    BlockADT prev = NULL;

    while (current != NULL && (current->size < nbytes))
    {
        prev = current;
        current = current->next;
    }

    if (current == NULL)
        return NULL;

    if (current->size == nbytes)
    {
        if (prev != NULL)
            prev->next = current->next;
        else
            head = current->next;

        usedHeapMemory += nbytes;
        memoryBlockCount++;
        return (void *)(current + 1); // Skip the block header
    }
    else
    {
        BlockADT newBlock = (BlockADT)((char *)current + nbytes + sizeof(BlockCDT));
        newBlock->size = current->size - nbytes - sizeof(BlockCDT);
        current->size = nbytes;

        if (prev != NULL)
            prev->next = newBlock;
        else
            head = newBlock;

        usedHeapMemory += nbytes;
        memoryBlockCount++;
        return (void *)(current + 1); // Skip the block header
    }
}

void free_memory_manager(void* ap) {
     if (ap == NULL)
        return;

    BlockADT block = (BlockADT)((char *)ap - sizeof(BlockCDT));
    usedHeapMemory -= block->size;
    memoryBlockCount--;

    // Add the freed block back to the free list
    block->next = head;
    head = block;
}

MemoryInfo *mem_info()
{
    MemoryInfo *info = (MemoryInfo *)memory_manager_malloc(sizeof(MemoryInfo));
    if (info == NULL)
    {
        return NULL;
    }

    info->memoryAlgorithmName = MEMORY_MANAGER_NAME;
    info->totalMemory = totalHeapMemory;
    info->occupiedMemory = usedHeapMemory;
    info->freeMemory = totalHeapMemory - usedHeapMemory;
    info->blocksUsed = memoryBlockCount;
    return info;
}
    // printf("\nUsando memory manager\n");
    // printf("VUELCO DE MEMORIA \n");
    // printf("\nMemoria Total: %d bytes\n\n", (uint32_t)total_heap_size * sizeof(header));

    // if (free_node == NULL) {
    //     printf("\nNo hay bloques de memoria disponibles.\n");
    //     return;
    // }
    // printf("Bloques libres:\n\n");

    // while (current != first || flag) {
    //     flag = FALSE;
    //     printf("\tBloque numero: %d\n", block_number);
    //     printf("\tBase:%x\n", (uint64_t)current);
    //     printf("\tBytes disponibles: %d\n\n", (int)current->data.size);

    //     current = current->data.ptr;
    //     block_number++;
    // }

    // printf("\n");

#endif
