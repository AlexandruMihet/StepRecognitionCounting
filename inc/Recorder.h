#ifndef _RECORDER_H
#define _RECORDER_H

#include <stdint.h>

#define MAX_BUFFER_SIZE 1200

void RecorderInit(uint8_t x, uint8_t y, uint8_t z);
void RecorderStart(void);
void RecorderCallback(void);
uint8_t RecorderFinished(void);
uint8_t* RecorderGetBuffer(void);
uint32_t RecorderGetSize(void);

#endif
