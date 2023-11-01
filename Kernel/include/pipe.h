#include "defs.h"
#include "memoryManager.h"
#include "queue.h"
#include "../scheduler/scheduler.h"

Pipe *pipeOpen();
int pipeClose(Pipe *pipe);
int pipeRead(Pipe *pipe, char *msg, int size);
int pipeWrite(Pipe *pipe, char *msg, int size);
