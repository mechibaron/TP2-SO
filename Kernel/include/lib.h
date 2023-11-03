#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void *memset(void *destination, int32_t character, uint64_t length);
void *memcpy(void *destination, const void *source, uint64_t length);
char *strcpy(const char *str);
unsigned int strlen(const char *str);
int strcmp(const char *p1, const char *p2);

unsigned int sys_readKey();

char *cpuVendor(char *result);

#endif  /* LIB_H */
