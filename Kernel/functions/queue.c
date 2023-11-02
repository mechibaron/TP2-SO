#include "../include/queue.h"


//funcion para la nueva queue de bloqueados
BlockedQueueADT newQueue() {
    BlockedQueueADT queue = memoryManagerAlloc(sizeof(BlockedQueueCDT));
    if (queue != NULL) {
        queue->head = NULL;
        queue->tail = NULL;
        queue->size = 0;
    }
    return queue;
}

//funcion para eliminar un proceso de la queue de bloqueados. Devuelve el PID
pid_t dequeuePid(BlockedQueueADT queue) {
    if (queue == NULL || queue->head == NULL) {
        return -1;
    }
    //apuntamos al primer elemento de la queue
    BlockedNode *node = queue->head;
    //actualizamos el puntero
    queue->head = node->next;
    pid_t pid = node->pid;
    memory_manager_free(node);
    queue->size--;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    return pid;
}

//funcion para agregar un proceso a la queue
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

//funcion para liberar la memoria de la queue
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
