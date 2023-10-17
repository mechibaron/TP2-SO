#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Tamaño máximo de la memoria
#define MEMORY_SIZE 1024

//Estructura para almacenar la informacion del estado de la memoria.
typedef struct MemoryStatus{
    size_t total_size;
    size_t free_size;
    size_t used_size;
} MemoryStatus;

// Estructura para representar un bloque de memoria
typedef struct Block {
    size_t size;
    bool is_free;
    struct Block* next;
    MemoryStatus* status;
} Block;

// Inicializa la memoria
void initialize_memory(Block* memory, size_t size) {
    memory->size = size;
    memory->is_free = true;
    memory->next = NULL;
    
    //inicializa el estado de la memoria
    MemoryStatus* status = (MemoryStatus*)malloc(sizeof(MemoryStatus));
    status->total_size = size;
    status->free_size = size;
    status->used_size = 0;
    memory->status = status;
}

// Asigna memoria
void* allocate_memory(Block* memory, size_t size) {
    Block* block = memory;
    while (block) {
        if (block->is_free && block->size >= size) {
            if (block->size > size) {
                // Dividir el bloque en dos si es más grande que el tamaño solicitado
                Block* buddy = (Block*)((char*)block + size);
                buddy->size = block->size - size;
                buddy->is_free = true;
                buddy->next = block->next;
                block->size = size;
                block->is_free = false;
                block->next = buddy;
            }
            block->is_free = false;
            return (void*)block;
        }
        block = block->next;
    }
    return NULL; // No se pudo asignar memoria
}

// Libera memoria
void deallocate_memory(Block* memory, void* ptr) {
    Block* block = (Block*)ptr;
    block->is_free = true;
    
    // Combinar bloques libres adyacentes
    while (block->next && block->next->is_free) {
        block->size += block->next->size;
        block->next = block->next->next;
    }
    
}

// Consultar el estado de la memoria
void memory_status(Block* memory) {
    memory->status->total_size = 0;
    memory->status->free_size = 0;
    memory->status->used_size = 0;
    
    Block* block = memory;
    while (block) {
        memory->status->total_size += block->size;
        if (block->is_free) {
            memory->status->free_size += block->size;
        } else {
            memory->status->used_size += block->size;
        }
        block = block->next;
    }
    
    printf("Total Memory: %lu\n", memory->status->total_size);
    printf("Used Memory: %lu\n", memory->status->free_size);
    printf("Free Memory: %lu\n", memory->status->used_size);
}

int main() {
    // Crear un bloque de memoria de tamaño MEMORY_SIZE
    Block memory;
    initialize_memory(&memory, MEMORY_SIZE);

    // Asignar y liberar memoria
    void* ptr1 = allocate_memory(&memory, 128);
    void* ptr2 = allocate_memory(&memory, 256);
    void* ptr3 = allocate_memory(&memory, 64);

    deallocate_memory(&memory, ptr2);
    deallocate_memory(&memory, ptr1);

    // Volver a asignar memoria
    void* ptr4 = allocate_memory(&memory, 200);

    // Consultar el estado de la memoria
    memory_status(&memory);
    
    free(memory.status);
    return 0;
}
