#include <unistd.h>
#include <stdint.h>
#include <defs.h>


int sem_open(uint32_t id, uint64_t initial_value);
int sem_wait(uint32_t id);
int sem_post(uint32_t id);
int sem_close(uint32_t id);
void sem_status();