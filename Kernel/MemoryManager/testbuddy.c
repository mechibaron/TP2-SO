#include <stdio.h>
#include <stdbool.h>

// Tamaño máximo de la memoria
#define MEMORY_SIZE 1024

// Estructura para representar un bloque de memoria
typedef struct Block {
    size_t size;
    bool is_free;
    struct Block* next;
} Block;

// Inicializa la memoria
void initialize_memory(Block* memory, size_t size) {
    memory->size = size;
    memory->is_free = true;
    memory->next = NULL;
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
    size_t total_memory = 0;
    size_t free_memory = 0;
    size_t used_memory = 0;
    
    Block* block = memory;
    while (block) {
        total_memory += block->size;
        if (block->is_free) {
            free_memory += block->size;
        } else {
            used_memory += block->size;
        }
        block = block->next;
    }
    
    printf("Total Memory: %lu\n", total_memory);
    printf("Used Memory: %lu\n", used_memory);
    printf("Free Memory: %lu\n", free_memory);
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

    return 0;
}