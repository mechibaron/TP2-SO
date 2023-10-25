#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>





//funciones para el estado del proceso
int set_state(uint64_t pid, int new_state);
int kill(uint64_t pid);
int block(uint64_t pid);
int ready(uint64_t pid);
int get_state(uint64_t pid);
int kill_current_foreground();

//info
typedef struct {
    int argc;
    char **argv;
    void *RBP;
    void *RSP;
    int pid;
    int state;
    int is_foreground;
    int ppid;
    int fd[2];
    uint64_t priority;  
} PCB;

typedef struct process_node{
    PCB pcb;
    struct process_node *next;
} process_node;

typedef struct process_list {
    uint32_t size;
    uint32_t process_ready_size;
    process_node *first;
    process_node *last;
} process_list;


//funciones que establecen prioridad
int set_priority(uint64_t pid, int new_priority);
int current_is_foreground();

//funciones para imprimir
void print_current();
void print_status();


//agregar un proceso a la lista de procesos
void queue_process(process_list *processes, process_node *process);
//funcion para eliminar un proceso de la lista de procesos
process_node *dequeue_process(process_list *processes);
//funcion que cheuquea si process_list esta vacia o no
int is_queue_empty(process_list *processes);
void simple_process(int argc, char** argv);
void _hlt(void);


int new_process(void (*function)(int, char**), int argc, char** argv, int is_foreground, int pipe_write);
void free_process(process_node* process);
int process_ready(uint64_t pid);

#endif
