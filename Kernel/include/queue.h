#ifndef QUEUE_H
#define QUEUE_H

#include "defs.h"
#include "memoryManager.h"

typedef struct BlockedNode {
    pid_t pid;
    struct BlockedNode *next;
} BlockedNode;

typedef struct BlockedQueueCDT {
    BlockedNode *head;
    BlockedNode *tail;
    unsigned int size;
} BlockedQueueCDT;

typedef BlockedQueueCDT *BlockedQueueADT;

BlockedQueueADT newQueue();
pid_t dequeuePid(BlockedQueueADT queue);
void enqueuePid(BlockedQueueADT queue, pid_t pid);
void freeQueue(BlockedQueueADT queue);

#endif
