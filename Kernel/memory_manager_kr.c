#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#define TRUE 1
#define FALSE !TRUE
#define MEMORY_MANAGER_NAME "Size allocation"

//limit for the align of the memory blocks: 8 bytes
typedef long Align;

//header is the struct to represent the header of the memory blocks
typedef union header{
    struct {
        //ptr to the next struct header in the list of mem blocks
        union header* ptr;
        //size of the mem block
        uint64_t size;
    } data;

Align x;
} header;

static header* base;
static header* free_node = NULL;

typedef struct info_mem_manager{
    char *mem_name;
    size_t free_mem_manager;
    size_t occupied_mem;
    size_t total_mem;
    unsigned int block_used;
}  info_mem_manager;

//to store the size of the available memory
uint64_t total_heap_size;

void create_memory_manager(char* heap_base, uint64_t heap_size){
    if(heap_base == NULL){
        return;
    }
    //#headers to put up in the free mem
    //heap_size + sizeof(header)-1 => le resto uno para asegurar que si heap_size no es un multiplo exacto del
    //tamanio de header, se siga obteniendo un valor que sea >= que el tamanio necesario para almacenar los bloques de mem.
    //la division calcula cuantos headers entran en la mem disponible.
    //el + 1 para asegurarnos de que siempre haya al menos un header disponible
    total_heap_size = (heap_size + sizeof(header)-1)/ sizeof(header)+1;
    free_node = base = (header*)heap_base;
    free_node->data.size = total_heap_size;
    free_node->data.ptr = free_node;
}

void* memory_manager_malloc(uint64_t malloc_bytes){
    if(malloc_bytes == 0){
        return NULL;
    }
    //para guardar la direc de mem asignada 
    void* result; 
    //para poder recorrer la lista de bloques de mem libres
    header *current_node, *previous_ptr;
    //calculamos la cantidad de unidades de malloc necesarias para almancenar incluyendo a header
    uint64_t malloc_units = (malloc_bytes + sizeof(header)-1) / sizeof(header)+1;
    //inicializamos el previous_ptr con free_node porque es el inicio de la lista de bloques de mem libres.
    previous_ptr = free_node;
    //controlamos si se encontro un bloque para la asignacion
    char node_found = TRUE;
    //recorremos la lista de bloques de mem libres que satisfaga malloc_units
    //si lo encuentra, queremos que lo asigne bien.
    for (current_node = previous_ptr->data.ptr; node_found; current_node=current_node->data.ptr){
        if(current_node->data.size >= malloc_units){
            //si es exactamente igual al tamanio necesario, se puede asignar todo el bloque sin dividirlo
            //usamos el puntero previous para omitirlo porque se asigna por completo
            if(current_node->data.size == malloc_units){
                previous_ptr->data.ptr = current_node->data.ptr;
            }
            //si es mayor, significa que el bloque se divide en dos partes
            else{
                //la primera parte se asigna y se achica el tamanio del bloque actual
                current_node->data.size -= malloc_units;
                //me muevo hacia la otra parte del bloque
                current_node += current_node->data.size;
                //pongo el tamanio en malloc_units
                current_node->data.size = malloc_units;
            }
            //actualizo para que apunte al ultimo bloque de mem libre encontrado
            free_node = previous_ptr;
            //result l establecemos en la direccion del primer byte del bloque asignado
            result = current_node + 1;
            //se pone en false para indicar que se encontro un bloque adecuado y parar la busqueda.
            node_found = FALSE;
        }
        //si recorrio toda la lista sin encontrar un bloque adecuado, retorna NULL
        if (current_node == free_node){
            return NULL;
        }
        //lo actualizamos para que apunte al current antes de seguir avanzando
        previous_ptr = current_node;

    }
    return result;
}

void free_memory_manager(void* block_blocked){
        // Chequeamos que el bloque que queremos liberar no sea nulo
    if (block_blocked == NULL) {
        return;
    }

    header* free_block = (header*)((char*)block_blocked - sizeof(header));
    if (free_block < base || free_block >= (base + total_heap_size)) {
        return;
    }

    header* current_node;
    char check_external = TRUE;

    for (current_node = free_node; (free_block <= current_node || free_block >= current_node->data.ptr) && !check_external; current_node = current_node->data.ptr) {
        if (current_node >= current_node->data.ptr && (free_block > current_node || free_block < current_node->data.ptr)) {
            check_external = FALSE;
        }
    }

    if (!check_external) {
        if (current_node + current_node->data.size == free_block) {
            current_node->data.size += free_block->data.size;
            current_node->data.ptr = free_block->data.ptr;
        } else {
            free_block->data.ptr = current_node->data.ptr;
            free_node = free_block;
        }
    } else {
        free_block->data.ptr = current_node->data.ptr;
        free_node = free_block;
    }
    // //chequeamos que el bloque que queremos liberar no sea nulo
    // if(block_blocked == NULL){
    //     return;
    // }
    // header *free_block, *current_node;
    // //inicializamos free block con la direccion del bloque que vamos a liberar
    // //le restamos 1 para que apunte al header del bloque
    // free_block = (header*)block_blocked-1;
    // //verificamos que no sea ni menor, ni se pase del rango de memoria
    // if(free_block < base || free_block >= (base+total_heap_size * sizeof(header))){
    //     return;
    // }
    // //queremos saber si el bloque que vamos a liberar es externo a la lista de bloques existentes
    // char check_external;
    // //recorremos la lista de bloques de mem libres para poner el liberado
    // //el bucle sigue mientras que free_block este fuera de rango y check_external sea false.
    // for(current_node = free_node; (free_block <= current_node || free_block >= current_node->data.ptr) && !check_external; current_node=current_node->data.ptr){
    // //    if(free_block == current_node || free_block == current_node->data.ptr){
    // //     return;
    // //    }
    //    if(current_node>= current_node->data.ptr && (free_block > current_node) || free_block < current_node->data.ptr){
    //     check_external = TRUE;
    //    }
    // }

    // if(!check_external){
    //     //chequeamos que no haya superposicion de bloques de memoria entre el free_block y el current_node
    //     if(current_node+current_node->data.size > free_block || free_block+free_block->data.size > current_node->data.ptr){
    //         return;
    //     }
    // }

    // //chequeamos que el bloque que se va a liberar se puede fusionar con el bloque siguiente al actual
    // if(free_block+free_block->data.size == current_node->data.ptr){
    //     //ajustamos el tamabio del que vamos a liberar con el siguiente bloque
    //     free_block->data.size += current_node->data.ptr->data.size;
    //     //acrtualizamos el puntero para que apunte al que sigue del siguiente bloque
    //     free_block->data.ptr = current_node->data.ptr->data.ptr;
    // }
    // //chequeamos si el bloque actual se puede fusionar con el que se va a liberar
    // //comparo si la direc final del bloque actual == a la direc de inicio del free_block
    // else if (current_node+current_node->data.size  == free_block){
    //     //ajustamos tamabios
    //     current_node->data.size += free_block->data.size;
    //     //actualizamos puntero
    //     current_node->data.ptr  = free_block->data.ptr;
    // }
    // else{
    //     //si llega aca es porque no se pueden fusionar
    //     //Entonces, establecemos el puntero del free_block para que apunte al siguiente del bloque actual
    //     free_block->data.ptr = current_node->data.ptr;
    // }

    // free_node = current_node;

}

info_mem_manager *info_memory_manager(){
    info_mem_manager *dump = memory_manager_malloc(sizeof(info_mem_manager));
    if(dump == NULL){
        return NULL;
    }
    dump->mem_name = MEMORY_MANAGER_NAME;
    //calculo la total mem
    dump->total_mem = total_heap_size * sizeof(header);
    //ponemos la memoria libre
    // dump->free_mem_manager = dump->total_mem; 

    size_t free_mem = 0;
    int block_number = 0;
    header* current = free_node;
    char flag = 1;
    while (current != free_node || flag) {
        flag = 0;
        free_mem += current->data.size * sizeof(header);
        // dump->free_mem_manager -= current->data.size * sizeof(header);
        block_number++;
        current = current->data.ptr;
    }

    dump->free_mem_manager = free_mem;
    dump->occupied_mem = dump->total_mem - free_mem;
    dump->block_used = block_number;

    return dump;

}
//TEST DE LA INFO
// int main() {
//     // Tamaño total de la memoria
//     uint64_t total_memory_size = 1024; // Cambia esto al tamaño deseado
//     char* memory_pool = (char*)malloc(total_memory_size);

//     // Inicializar el administrador de memoria
//     create_memory_manager(memory_pool, total_memory_size);

//     // Asignar algunos bloques de memoria
//     void* ptr1 = memory_manager_malloc(64);  // Asignar 64 bytes
//     void* ptr2 = memory_manager_malloc(128); // Asignar 128 bytes

//     // Imprimir información del estado de la memoria
//     info_mem_manager* info = info_memory_manager();
//     if (info != NULL) {
//         printf("Memory Manager Name: %s\n", info->mem_name);
//         printf("Total Memory: %zu bytes\n", info->total_mem);
//         printf("Free Memory: %zu bytes\n", info->free_mem_manager);
//         printf("Occupied Memory: %zu bytes\n", info->total_mem - info->free_mem_manager);
//         printf("Blocks Used: %u\n", info->block_used);
//     } else {
//         printf("Error: info_memory_manager returned NULL.\n");
//     }

//     // Liberar algunos bloques de memoria
//     free_memory_manager(ptr1);
//     free_memory_manager(ptr2);

//     // Imprimir información del estado de la memoria nuevamente
//     info = info_memory_manager();
//     if (info != NULL) {
//         printf("\nMemory Manager Name: %s\n", info->mem_name);
//         printf("Total Memory: %zu bytes\n", info->total_mem);
//         printf("Free Memory: %zu bytes\n", info->free_mem_manager);
//         printf("Occupied Memory: %zu bytes\n", info->total_mem - info->free_mem_manager);
//         printf("Blocks Used: %u\n", info->block_used);
//     } else {
//         printf("Error: info_memory_manager returned NULL.\n");
//     }

//     free(memory_pool); // Liberar la memoria principal
//     return 0;
// }

//TEST DE LA ASIGNACION Y LA LIBERACION
// int main(){
//     char memory_area[1024];
//     create_memory_manager(memory_area, sizeof(memory_area));

//     int *ptr1 = memory_manager_malloc(sizeof(int));
//     if (ptr1 == NULL) {
//         printf("Error: No se pudo asignar memoria para ptr1.\n");
//         return 1;
//     }

//     printf("ptr1: %p\n", (void *)ptr1);

//     free_memory_manager(ptr1);
//     printf("ptr1 liberado\n");

//     int *ptr2 = memory_manager_malloc(sizeof(int));
//     if (ptr2 == NULL) {
//         printf("Error: No se pudo asignar memoria para ptr2.\n");
//         return 1;
//     }

//     printf("ptr2: %p\n", (void *)ptr2);

//     free_memory_manager(ptr2);
//     printf("ptr2 liberado\n");

//     // Obtener información sobre la memoria
//     info_mem_manager *info = info_memory_manager();
//     printf("Memoria libre: %zu bytes\n", info->free_mem_manager);
//     printf("Bloques utilizados: %u\n", info->block_used);

//     // Limpieza de memoria al final
//     memory_manager_malloc(sizeof(info_mem_manager)); // Esta llamada no la guardamos, pero libera la última estructura info_mem_manager

//     return 0;
// }