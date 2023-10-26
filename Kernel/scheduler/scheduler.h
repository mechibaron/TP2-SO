#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int pid_t;

// Estructura de un proceso (PCB)
typedef struct ProcessControlBlock {
    pid_t pid;
    uint64_t rip;
    int priority;
    int quantumsLeft;
    int status;
    uint64_t stackBase;
    unsigned int argc;
    char** argv;   
} PCB;

// Estructura de un nodo de la lista de procesos
typedef struct Node {
    PCB process;
    struct Node* next;
} Node;



#endif
