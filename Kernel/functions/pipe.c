
#include "../include/pipe.h"

pipeList pipesList = NULL;

//funcion para abrir un pipe nuevo
Pipe *pipeOpen()
{
    //incluimos en el newpipe las variables de pipe e inicializamos
    Pipe *newPipe = (Pipe *)memoryManagerAlloc(sizeof(Pipe));
    newPipe->isReadOpen = 1;
    newPipe->isWriteOpen = 1;
    newPipe->readQueue = newQueue();
    newPipe->writeQueue = newQueue();
    newPipe->processCount = 1;

    pipeNode *newPipeNode = (pipeNode *)memoryManagerAlloc(sizeof(pipeNode));
    newPipeNode->pipe = newPipe;
    newPipeNode->next = pipesList;
    newPipeNode->previous = NULL;
    pipesList = newPipeNode;
    return newPipe;
}

pipeNode *findPipeInList(Pipe *pipe) {
    pipeNode *current = pipesList;
    while (current != NULL && current->pipe != pipe) {
        current = current->next;
    }
    return current;
}

void cleanupAndRemovePipe(pipeNode *node) {
    if (node == NULL) {
        return;
    }

    freeQueue(node->pipe->readQueue);
    freeQueue(node->pipe->writeQueue);
    
    if (node->next != NULL) {
        node->next->previous = node->previous;
    }
    
    if (node->previous != NULL) {
        node->previous->next = node->next;
    }

    memory_manager_free(node);
    memory_manager_free(node->pipe);
}


// TODO tengo que ver como liberar las queues
int pipeClose(Pipe *pipe) {
    // Decrement the process count, if greater than 1
    if (pipe->processCount > 1) {
        pipe->processCount--;
    }

    // Encontramos el pipe en la lista
    pipeNode *current = findPipeInList(pipe);
    
    if (current == NULL) {
        return 0; 
    }

    // Limpiamos el pipe
    cleanupAndRemovePipe(current);

    return 1;
}

//devuelve la cantidad de chars leidos y sino -1
int pipeReadData(Pipe *pipe, char *msg, int size) {
    if (!pipe->isReadOpen) {
        return -1;
    }

    int i;
    pid_t currentPid;
    for (i = 0; i < size; i++) {
        //verificamos que el readIndex sea igual al writeIndex
        if (pipe->readIndex == pipe->writeIndex) {
            //si lo son, es porque no hay datos para leer en el pipe
            currentPid = getCurrentPid();
            //se agrega a la readQueue hasta que haya datos disponibles para leer
            enqueuePid(pipe->readQueue, currentPid);
            //el proceso actual se blquea
            blockProcess(currentPid);
        }
        //si hay datos, se lee un char y se almacena en msg
        msg[i] = pipe->data[pipe->readIndex % PIPESIZE];
        pipe->readIndex++;
        //despues de cada lectura verificamos si hay procesos bloqueados esperando
        //para escribir en el tubo
        while ((currentPid = dequeuePid(pipe->writeQueue)) != -1) {
            //si hay, se desbloquean y se sacan de la writeQueue
            unblockProcess(currentPid);
        }
    }

    return i;
}


//devuelve la cantidad de chars escritos y sino -1
//funciona de manera similar que el pipeReadData
int pipeWriteData(Pipe *pipe, const char *msg, int size) {
    if (!pipe->isWriteOpen) {
        return -1;
    }

    int i;
    pid_t currentPid;

    for (i = 0; i < size; i++) {
        if (pipe->writeIndex == pipe->readIndex + PIPESIZE) {
            currentPid = getCurrentPid();
            enqueuePid(pipe->writeQueue, currentPid);
            blockProcess(currentPid);
        }

        pipe->data[pipe->writeIndex % PIPESIZE] = msg[i];
        pipe->writeIndex++;

        while ((currentPid = dequeuePid(pipe->readQueue)) != -1) {
            unblockProcess(currentPid);
        }
    }

    return 1;
}

