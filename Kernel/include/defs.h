/***************************************************
  Defs.h
****************************************************/

#ifndef _defs_
#define _defs_

#include <stdint.h>

#define NULL ((void *)0)

/* Flags para derechos de acceso de los segmentos */
#define ACS_PRESENT     0x80            /* segmento presente en memoria */
#define ACS_CSEG        0x18            /* segmento de codigo */
#define ACS_DSEG        0x10            /* segmento de datos */
#define ACS_READ        0x02            /* segmento de lectura */
#define ACS_WRITE       0x02            /* segmento de escritura */
#define ACS_IDT         ACS_DSEG
#define ACS_INT_386 	  0x0E		        /* Interrupt GATE 32 bits */
#define ACS_INT         ( ACS_PRESENT | ACS_INT_386 )


#define ACS_CODE        (ACS_PRESENT | ACS_CSEG | ACS_READ)
#define ACS_DATA        (ACS_PRESENT | ACS_DSEG | ACS_WRITE)
#define ACS_STACK       (ACS_PRESENT | ACS_DSEG | ACS_WRITE)

#define PIPESIZE 512

typedef int pid_t;
typedef struct blockednode
{
    pid_t pid;
    struct blockednode *next;
} BlockedNode;

typedef struct
{
    BlockedNode *first;
    BlockedNode *last;
    unsigned int qty;
} BlockedQueueCDT;

typedef BlockedQueueCDT *BlockedQueueADT;

typedef struct Pipe
{
    char data[PIPESIZE];
    unsigned int processCount;
    unsigned int openR;
    unsigned int openW;
    uint64_t indexR;
    uint64_t indexW;
    BlockedQueueADT queueWriteBlocked;
    BlockedQueueADT queueReadBlocked;
} Pipe;

typedef struct pipeNode
{
    Pipe *pipe;
    struct pipeNode *next;
    struct pipeNode *previous;
} pipeNode;

typedef pipeNode *pipeList;

#endif
