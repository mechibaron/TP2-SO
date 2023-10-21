#ifdef SCHEDULER_H
#define SCHEDULER_H

//funciones del scheduler
void start_scheduler();


//funciones para el estado del proceso
int set_state(uint64_t pid, int new_state);
int kill(uint64_t pid);
int block(uint64_t pid);
int ready(uint64_t pid);
int get_state(uint64_t pid);
int kill_current_foreground();




//funciones que establecen prioridad
int set_priority(uint64_t pid, int new_priority);
int current_is_foreground();

//funciones para imprimir
void print_current();
void print_status();

#endif
