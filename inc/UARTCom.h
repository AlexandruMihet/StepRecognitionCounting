#ifndef _USARTCOM_H_
#define _USARTCOM_H_

#include <stdint.h>

void Com_Init(uint32_t baud);

uint8_t Com_HasByte(void);
uint8_t Com_GetByte(void);

uint8_t Com_ReadReady(void);
uint8_t Com_WriteReady(void);
void Com_ReadByteArray(uint8_t* buffer, uint32_t length, void (*callback)(void));
void Com_WriteByteArray(uint8_t* buffer, uint32_t length, void (*callback)(void));

char stdin_getchar(void);
void stdout_putchar(char ch);
void stdout_putstr(char* str, char max);

#endif
