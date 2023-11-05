#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <idtLoader.h>
#include <naiveConsole.h>
#include <lib.h>
#include <scheduler.h>
#include <inforeg.h>

void keyboard_handler();
uint64_t readBuffer(char* output, uint64_t count);
// int getChar();
// void clear_buffer();

#endif  /* __KEYBOARD_H__ */
