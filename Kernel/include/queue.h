#ifndef QUEUE_H
#define QUEUE_H

#include "../include/defs.h"
#include "../include/memoryManager.h"

// typedef int pid_t;
// typedef struct blockednode
// {
//     pid_t pid;
//     struct blockednode *next;
// } BlockedNode;
//
// typedef struct
// {
//     BlockedNode *first;
//     BlockedNode *last;
//     unsigned int qty;
// } BlockedQueueCDT;

typedef BlockedQueueCDT *BlockedQueueADT;

BlockedQueueADT newQueue();
pid_t dequeuePid(BlockedQueueADT queue);
void enqueuePid(BlockedQueueADT queue, pid_t pid);
void freeQueue(BlockedQueueADT queue);

#endif
