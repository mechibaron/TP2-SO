#ifdef BUDDY_MM

#include <memoryManager.h>
#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 1024
#define MIN_ALLOCATION 32

// Estructura para representar un bloque de memoria
typedef struct Block {
    size_t size;
    struct Block *next;
    int is_free;
} Block;

Block memory[MEMORY_SIZE];

// Inicializa la memoria
void createMemory(size_t size) {
    // Inicializa la memoria con el tamaño proporcionado
    memory[0].size = size;
    memory[0].next = NULL;
    memory[0].is_free = 1;
}

// Divide un bloque en dos partes si es más grande que el tamaño solicitado
void split(Block *block, size_t size) {
    Block *newBlock = (Block *)((char *)block + size);
    newBlock->size = block->size - size;
    newBlock->next = block->next;
    newBlock->is_free = 1;
    block->size = size;
    block->next = newBlock;
}

// Asigna memoria
void *memoryManagerAlloc(size_t nbytes) {
    if (nbytes == 0 || nbytes > MEMORY_SIZE) {
        return NULL;
    }

    size_t allocSize = 1;
    while (allocSize < nbytes) {
        allocSize *= 2;
    }

    Block *block = memory;
    while (block) {
        if (block->is_free && block->size >= allocSize) {
            if (block->size > allocSize) {
                split(block, allocSize);
            }
            block->is_free = 0;
            return (void *)(block + 1); // +1 para saltar la información de bloque
        }
        block = block->next;
    }

    return NULL; // No se pudo asignar memoria
}

// Combina bloques adyacentes libres
void merge() {
    Block *block = memory;
    while (block && block->next) {
        if (block->is_free && block->next->is_free) {
            block->size += block->next->size;
            block->next = block->next->next;
        } else {
            block = block->next;
        }
    }
}

// Libera memoria
void memory_manager_free(void *ap) {
    if (ap == NULL) {
        return;
    }

    Block *block = (Block *)ap - 1;
    if (block->is_free) {
        return;
    }

    block->is_free = 1;
    merge();
}

// Consulta el estado de la memoria
MemoryInfo *mem_info() {
    Block *block = memory;
    int blockCount = 0;
    int freeMemory = 0;
    int usedMemory = 0;

    while (block) {
        blockCount++;
        if (block->is_free) {
            freeMemory += block->size;
        } else {
            usedMemory += block->size;
        }
        block = block->next;
    }

    // Creamos una estructura MemoryInfo con la información del estado de la memoria
    MemoryInfo *info = (MemoryInfo *)malloc(sizeof(MemoryInfo));
    if (info != NULL) {
        info->memoryAlgorithmName = MEMORY_MANAGEMENT_NAME; // Asigna el nombre de tu algoritmo aquí
        info->totalMemory = MEMORY_SIZE;
        info->occupiedMemory = usedMemory;
        info->freeMemory = freeMemory;
        info->blocksUsed = blockCount;
    }

    return info;
}

int main() {
// Crear memoria y asignar bloques
createMemory(MEMORY_SIZE);
void *ptr1 = memoryManagerAlloc(128);
void *ptr2 = memoryManagerAlloc(256);
void *ptr3 = memoryManagerAlloc(64);

// Consultar el estado de la memoria antes de liberar
MemoryInfo *info = mem_info();
printf("Memory Algorithm: %s\n", info->memoryAlgorithmName);
printf("Total Memory: %lu\n", info->totalMemory);
printf("Used Memory: %lu\n", info->occupiedMemory);
printf("Free Memory: %lu\n", info->freeMemory);
printf("Blocks Used: %u\n", info->blocksUsed);

// Liberar memoria
memory_manager_free(ptr2);
memory_manager_free(ptr1);

// Consultar el estado de la memoria después de liberar
info = mem_info();
printf("Memory Algorithm: %s\n", info->memoryAlgorithmName);
printf("Total Memory: %lu\n", info->totalMemory);
printf("Used Memory: %lu\n", info->occupiedMemory);
printf("Free Memory: %lu\n", info->freeMemory);
printf("Blocks Used: %u\n", info->blocksUsed);

// Liberar la estructura MemoryInfo
free(info);

return 0;
}

#endif