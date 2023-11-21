#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <unistd.h>
#include <stdint.h>

typedef struct blockCDT
{
    size_t size;  // Number of bytes allocated
    size_t unused;
    struct blockCDT *prev;          
    struct blockCDT *next; // Pointer to next block
} BlockCDT;

typedef BlockCDT *BlockADT;

typedef struct memoryInfo
{
    char *memoryAlgorithmName;
    size_t freeMemory;
    size_t occupiedMemory;
    size_t totalMemory;
    unsigned int blocksUsed;
} MemoryInfo;

typedef long Align;
typedef union header {
    struct {
        union header* ptr;
        uint64_t size;
    } data;

    Align x;
} header;

void createMemory(size_t size);
void *memory_manager_malloc(size_t nbytes);
void free_memory_manager(void *ap);
void memory_dump();
MemoryInfo *mem_info();

#endif