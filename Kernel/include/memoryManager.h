#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <unistd.h>
typedef struct memoryInfo
{
    char *memoryAlgorithmName;
    size_t freeMemory;
    size_t occupiedMemory;
    size_t totalMemory;
} MemoryInfo;

void createMemory(size_t size);
void *memoryManagerAlloc(size_t nbytes);
void memory_manager_free(void *ap);
MemoryInfo *mem_info();

#endif