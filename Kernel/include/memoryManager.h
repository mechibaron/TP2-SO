#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <unistd.h>
typedef struct memoryInfo
{
    char *memoryAlgorithmName;
    size_t freeMemory;
    size_t occupiedMemory;
    size_t totalMemory;
    unsigned int blocksUsed;
} MemoryInfo;

void createMemory(size_t size);
void *memory_manager_malloc(size_t nbytes);
void free_memory_manager(void *ap);
MemoryInfo *mem_info();

#endif