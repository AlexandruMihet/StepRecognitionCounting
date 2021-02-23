#ifndef _PERIODICTIMER_H_
#define _PERIODICTIMER_H_ 

#include <stdint.h>

extern volatile char timerEvent;

//Periodic interrupt timer PIT functions
void timer0_init(uint32_t interval);
void PIT_IRQHandler(void);

//Clock configuration
void config_sys_clock(void);

#endif
