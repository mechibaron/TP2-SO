#include "scheduler.h"
#include <stdint.h>

#define STACK_SIZE (1024*4)
#define MAX_PRIORITY 10
#define DEFAULT_BACKGROUND_PRIORITY 1
#define DEFAULT_FOREGROUND PRIORITY 2

typedef struct {
    uint64_t gs;
    uint64_t fs;
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    uint64_t rip;
    uint64_t cs;
    uint64_t eflags;
    uint64_t rsp;
    uint64_t ss;
    uint64_t base;
} stack_frame;

//info
typedef struct {
    int argc;
    char **argv;
    void *RBP;
    void *RSP;
    int pid;
    int state;
    int is_foreground;
    int ppid;
    int fd[2];
    uint64_t priority;  
} process_info;

typedef struct process_node{
    process_info info;
    struct process_node *next;
} process_node;

typedef struct process_list {
    uint32_t size;
    uint32_t process_ready_size;
    process_node *first;
    process_node *last;
} process_list;


void start_scheduler(){

}