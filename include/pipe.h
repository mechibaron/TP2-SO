#ifndef PIPE_H
#define PIPE_H

#include "./scheduler/scheduler.h"
#include "defs.h"
// #include <memoryManager.h>
// #include "queue.h"

// Pipes
typedef struct Pipe{
    char data[PIPESIZE];
    unsigned int processCount;
    unsigned int isReadOpen;
    unsigned int isWriteOpen;
    uint64_t readIndex;
    uint64_t writeIndex;
    BlockedQueueADT writeQueue;
    BlockedQueueADT readQueue;
} Pipe;

typedef struct pipeNode{
    Pipe *pipe;
    struct pipeNode *next;
    struct pipeNode *previous;
} pipeNode;

typedef pipeNode *pipeList;

// // Blocked Queue
typedef struct blockednode{
    pid_t pid;
    struct blockednode *next;
} BlockedNode;

typedef struct{
    BlockedNode *first;
    BlockedNode *last;
    unsigned int qty;
} BlockedQueueCDT;

typedef BlockedQueueCDT *BlockedQueueADT;

Pipe *pipeOpen();
int pipeClose(Pipe *pipe);
int pipeReadData(Pipe *pipe, char *msg, int size);
int pipeWriteData(Pipe *pipe, const char *msg, int size);

#endif