#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>
#include <naiveConsole.h>
#include <lib.h>
#include "keyboard.h"
#include "../scheduler/scheduler.h"
#include "inforeg.h"
#include <defs.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define PIPEOUT 3
#define PIPEIN 4

void _syscallHandler(void);


typedef struct
{
    uint8_t day, month, year;
    uint8_t hours, minutes, seconds;
} sysTime_t;

uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rax, uint64_t *registers);

#endif
