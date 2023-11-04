#ifndef _TIME_H_
#define _TIME_H_

#include "idtLoader.h"
#include "naiveConsole.h"
#include "../scheduler/scheduler.h"

void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
int miliseconds_elapsed();

#endif  /* _TIME_H_ */
