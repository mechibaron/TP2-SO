#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
// #include <naiveConsole.h>
// #include <interrupts.h>
// #include <timerDriver.h>
#include "../include/defs.h"
// #include <queue.h>
#define READY 0
#define BLOCKED 1
#define FDS 10
#define OPEN 1
#define CLOSED 0
#define PIPESIZE 512

// // Blocked Queue
// typedef struct blockednode
// {
//     pid_t pid;
//     struct blockednode *next;
// } BlockedNode;
// typedef struct
// {
//     BlockedNode *first;
//     BlockedNode *last;
//     unsigned int qty;
// } BlockedQueueCDT;

// typedef BlockedQueueCDT *BlockedQueueADT;

// Pipes
// typedef struct Pipe
// {
//     char data[PIPESIZE];
//     unsigned int processCount;
//     unsigned int openR;
//     unsigned int openW;
//     uint64_t indexR;
//     uint64_t indexW;
//     BlockedQueueADT queueWriteBlocked;
//     BlockedQueueADT queueReadBlocked;
// } Pipe;
//
// typedef struct pipeNode
// {
//     Pipe *pipe;
//     struct pipeNode *next;
//     struct pipeNode *previous;
// } pipeNode;
// typedef pipeNode *pipeList;

typedef struct
{
    unsigned int mode;
} fd_t;

typedef unsigned int priority_t;
typedef unsigned int status_t;

typedef struct
{
    pid_t pid;
    priority_t priority;
    int newPriority;
    status_t status;
    unsigned int quantumsLeft;
    uint64_t rsp;
    uint64_t stackBase;
    BlockedQueueADT blockedQueue;
    fd_t fileDescriptors[FDS];
    // 0: STDIN, 1: STDOUT, 2:STDERR
    // 3: PIPEW, 4: PIPER
    Pipe *pipe;
    unsigned int lastFd;
    unsigned int argc;
    char **argv;
} PCB;
typedef struct node
{
    PCB process;
    struct node *next;
} Node;

typedef Node *Queue;
typedef struct processInfo
{
    pid_t pid;
    priority_t priority;
    uint64_t stackBase;
    status_t status;
    struct processInfo *next;
} processInfo;

void dummyProcess();
void createScheduler();
PCB *getProcess(pid_t pid);
uint64_t getCurrentPid();
int blockProcess(pid_t pid);
int unblockProcess(pid_t pid);
pid_t createProcess(uint64_t rip, int argc, char *argv[]);
void nextProcess();
int prepareDummyForWork(pid_t pid);
uint64_t contextSwitch(uint64_t rsp);
int killProcess(int returnValue, char autokill);
int changePriority(pid_t pid, int priorityValue);
int yieldProcess();
processInfo * getProccessesInfo();



#endif // SCHEDULER_H
