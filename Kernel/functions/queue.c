#include "../include/queue.h"

// Esto es lo que usa un proceso para determinar los procesos bloqueados por el mismo
// Ejemplo: lo usaria el semaforo para saber quienes estan bloqueados esperando al semaforo


BlockedQueueADT newQueue() {
    BlockedQueueADT queue = memoryManagerAlloc(sizeof(BlockedQueueCDT));
    if (queue != NULL) {
        queue->head = NULL;
        queue->tail = NULL;
        queue->size = 0;
    }
    return queue;
}

pid_t dequeuePid(BlockedQueueADT queue) {
    if (queue == NULL || queue->head == NULL) {
        return -1;
    }
    BlockedNode *node = queue->head;
    queue->head = node->next;
    pid_t pid = node->pid;
    memory_manager_free(node);
    queue->size--;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    return pid;
}

void enqueuePid(BlockedQueueADT queue, pid_t pid) {
    if (queue == NULL) {
        return;
    }
    BlockedNode *newNode = memoryManagerAlloc(sizeof(BlockedNode));
    if (newNode == NULL) {
        return;
    }
    newNode->pid = pid;
    newNode->next = NULL;
    if (queue->tail == NULL) {
        queue->head = newNode;
        queue->tail = newNode;
    } else {
        queue->tail->next = newNode;
        queue->tail = newNode;
    }
    queue->size++;
}

void freeQueue(BlockedQueueADT queue) {
    if (queue == NULL) {
        return;
    }
    while (queue->head != NULL) {
        BlockedNode *node = queue->head;
        queue->head = node->next;
        memory_manager_free(node);
    }
    memory_manager_free(queue);
}
