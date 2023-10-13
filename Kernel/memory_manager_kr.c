#include <stddef.h>
#include <stdint.h>


#define TRUE 1
#define FALSE !TRUE

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
    total_heap_size = (heap_size + sizeof(header)-1 / sizeof(header)+1);
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
}

void *free_memory_manager(void *block_blocked){
    //chequeamos que el bloque que queremos liberar no sea nulo
    if(block_blocked == NULL){
        return;
    }
    //recorremos la lista de bloques libres para poder meter el bloque liberado
    header *free_block, *current_node;
    //inicializamos free block con la direccion del bloque que vamos a liberar
    //le restamos 1 para que apunte al header del bloque
    free_block = (header*)block_blocked-1;
    //verificamos que no sea ni menor, ni se pase del rango de memoria
    if(free_block < base || free_block >= (base+total_heap_size * sizeof(header))){
        return;
    }

}

void info_memory_manager(){

}
