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
