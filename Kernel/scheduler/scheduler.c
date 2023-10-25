#include "scheduler.h"


#define STACK_SIZE (1024*4)
#define MAX_PRIORITY 10
#define DEFAULT_BACKGROUND_PRIORITY 1
#define DEFAULT_FOREGROUND PRIORITY 2
#define BACKGROUND 0
#define READY 0
#define FINISHED 1
#define BLOCKED 2


static uint64_t cycles_left;

static void inactive_process(int argc, char** argv);
static process_node* current_process = NULL;
static process_list* processes;
static process_node* base_process;
static process_node* get_process(uint64_t pid);


typedef struct {
    uint64_t gs;
    uint64_t fs;
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    uint64_t rip;
    uint64_t cs;
    uint64_t eflags;
    uint64_t rsp;
    uint64_t ss;
    uint64_t base;
} stack_frame;




void start_scheduler(){
    processes = malloc(sizeof(process_list));
    if(processes == NULL){
        return;
    }
    //inicializamos los campos de la estructura de process_list 
    processes->size = 0;
    processes->process_ready_size = 0;
    processes->first = NULL;
    processes->last = NULL;

    //creamos el proceso inicial
    char* argv[] = {"Initial Task"};
    new_process(&inactive_process, 1, argv, BACKGROUND, 0);

    //eliminamos un proceso de la lista de procesos
    base_process = dequeue_process(processes);

}

void *scheduler(void* sp){
    if(current_process != NULL){
        //si no es nulo es porque hay un proceso en ejecucion
        if(current_process->pcb.state == READY && cycles_left > 0){
            cycles_left --;
            //retornamos el puntero al registro de la pila actual
            return sp;
        }
        //actualizamos el puntero de pila del proceso actual
        current_process->pcb.RSP = sp;

        //verificamos si el proceso en ejecucion no es el proceso base
        if(current_process->pcb.pid != base_process->pcb.pid){
            //si el proceos termino la ejecucion esta listo para limpiarlo
            if(current_process->pcb.state == FINISHED){
                //obtenemos un puntero al proceso padre del proceso en ejecucion
                process_node* parent = get_process(current_process->pcb.ppid);
                //verificamos:
                //si el puntero al parent no es nulo -> el proceso en ejecucion tiene un parent
                //si el proceso en ejecucion esta en primer plano
                //si el proceso parent esta bloqueado
                if(parent != NULL && current_process->pcb.is_foreground && parent->pcb.state == BLOCKED){
                    //si se cumplen las condiciones, ponemos al proceso parent en ready para seguir la ejecucion
                    process_ready(parent->pcb.pid);
                }
                //liberamos la memoria ocupada por el proceso en ejecucion
                free_process(current_process);
            }
            else{
                //si no termino el proceso en ejecucion, lo tenemos que volver a meter en la cola
                queue_process(processes, current_process);
            }

        }
    }

    if(processes->process_ready_size > 0){
        //si hay procesos listos, tomamos el proximo proceso de la cola y la ponemos en current_process
        current_process = dequeue_process(processes);
        //entra en el ciclo hasta que el current este ready
        while(current_process->pcb.state != READY){
            if(current_process->pcb.state == FINISHED){
                //obtenemos el proceso padre y verifica si debe desbloquearse
                process_node* parent = get_process(current_process->pcb.ppid);
                //si cumple que el padre existe, el proceso es foreground y el parent esta bloqueado
                if(parent != NULL && current_process->pcb.is_foreground && parent->pcb.state == BLOCKED){
                    //si las cumple llamamos a ready con el proceso padre
                    process_ready(parent->pcb.pid);
                }
                //lo liberamos
                free_process(current_process);
            }
            
            if(current_process->pcb.state == BLOCKED){
                //si esta bloqueado lo volvemos a meter en la cola
                queue_process(processes, current_process);
            }
            //tomamos el siguiente proceso de la cola
            current_process = dequeue_process(processes);
        }
        
    } 
    else {
            //si no hay procesos listos vuelve al proceso base
            current_process = base_process;
        }
    
    cycles_left = current_process->pcb.priority;

    //devolvemos el registro de pila del proceso actual
    return current_process->pcb.RSP;

}

int set_state(uint64_t pid, int new_state) {
    process_node* process = get_process(pid);

    if (process == NULL || process->pcb.state == FINISHED) {
        return -1;
    }

    if (process == current_process) {
        process->pcb.state = new_state;
        return process->pcb.pid;
    }
    //chequeamos si hay que incrementar o no el process_ready_size
    if (process->pcb.state != READY && new_state == READY) {
        processes->process_ready_size++;
    }
    if (process->pcb.state == READY && new_state != READY) {
        processes->process_ready_size--;
    }

    process->pcb.state = new_state;

    return process->pcb.pid;
}


static process_node* get_process(uint64_t pid){
    //si es current retorno
    if(current_process != NULL && current_process->pcb.pid  ==  pid){
        return current_process;
    }

    process_node* process = processes->first;
    //se ejecuta hasta que no haya procesos en la lista para ver
    while(process != NULL){
        //verificamos que el PID del actual sea igual al prporcionado
        if(process->pcb.pid == pid){
            //si es asi es porque se encontro el proceso buscado y lo retornamos
            return process;
        }
        //actualizamos el puntero para que apunte al siguiente
        process = (process_node*)process->next;
    }
    return NULL;
}

//funcion para mantener a la CPU ocupada cuando  no hay otras tareas activas
static void inactive_process(int argc, char** argv){
    while(1){
        _hlt();
    }
}

void queue_process(process_list *processes, process_node *process){
    process->next = NULL;
    
    if(is_queue_empty(processes)){
        processes->first = process;
        processes->last = process;
    }
    else{
        processes->last->next = process;
        processes->last = process;
    }

    if(process->pcb.state == READY){
        processes->process_ready_size++;
    }
    processes->size++;
}

process_node *dequeue_process(process_list *processes){
    if(is_queue_empty(processes)){
        return NULL;
    }

    process_node *to_return = processes->first;
    processes->first = to_return->next;

    if(to_return->pcb.state == READY){
        processes->process_ready_size--;
    }

    to_return->next = NULL;
    processes->size--;

    return to_return;
}

int is_queue_empty(process_list *processes){
    processes->process_ready_size;
}

int process_ready(uint64_t pid){
    return set_state(pid, READY);
}

// //funcion de test
// void simple_process(int argc, char** argv) {
//     for (int i = 0; i < 5; i++) {
//         printf("Process %s: Iteration %d\n", argv[0], i);
//     }
// }


// int main() {
//     start_scheduler();

//     // Crear dos procesos
//     char* argv1[] = {"Process 1"};
//     char* argv2[] = {"Process 2"};
//     int pid1 = new_process(&simple_process, 1, argv1, BACKGROUND, 0);
//     int pid2 = new_process(&simple_process, 1, argv2, BACKGROUND, 0);

//     // Ejecutar el planificador en un bucle
//     for (int i = 0; i < 10; i++) {
//         scheduler(NULL);

//         if (i == 3) {
//             // Bloquear el proceso 1 después de 3 iteraciones
//             block(pid1);
//         } else if (i == 6) {
//             // Desbloquear el proceso 1 después de 6 iteraciones
//             ready(pid1);
//         }
//     }

//     return 0;
// }


// int new_process();nombre