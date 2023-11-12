#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include <stdarg.h>
#include <systemCalls.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
char *cpuVendor(char *result);
void acquire(int *lock);
void release(int *lock);
extern int exchange(int *, int);
void printf(char * string , ...);
uint8_t getSeconds();
uint8_t getMinutes();
uint8_t getHours();
uint8_t getDay();
uint8_t getMonth();
uint8_t getYear();
uint8_t read_port(uint8_t key);
char *cpuVendor(char *result);
void tick();
void loadUserland(uint64_t * address, uint64_t * rsp);
char * strcpy(const char * str);
unsigned int strlen(const char *str);
int strcmp (const char *p1, const char *p2);

#endif  /* LIB_H */