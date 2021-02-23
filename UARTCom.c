#include <MKL25Z4.h>

#include "UARTCom.h"


#define BUS_CLOCK (48000000)


volatile static uint8_t last_byte;
volatile static uint8_t has_byte;

static uint8_t* tx_buff;
static uint32_t tx_count;
static uint32_t tx_ind;
static uint8_t tx_active;
static void (*tx_call)(void);

static uint8_t* rx_buff;
static uint32_t rx_count;
static uint32_t rx_ind;
static void (*rx_call)(void);

void UART0_IRQHandler(void);

void Com_Init(uint32_t baud) {
	//https://learningmicro.wordpress.com/serial-communication-interface-using-uart/
	uint32_t divisor = baud;
	
	SIM->SCGC5 |= SIM_SCGC5_PORTA(1);
 
	PORTA_PCR1 |=  PORT_PCR_MUX(2); /* PTA1 as ALT2 (UART0) */
	PORTA_PCR2 |=  PORT_PCR_MUX(2); /* PTA2 as ALT2 (UART0) */
	
	// Select MCGFLLCLK as UART0 clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
 
	// Enable UART0 Clock
	SIM->SCGC4 |= SIM_SCGC4_UART0(1);
	
	// Configure Baud Rate
	divisor = BUS_CLOCK /(divisor *16);
	UART0->BDH = UART_BDH_SBR(divisor>>8);
	UART0->BDL = UART_BDL_SBR(divisor);
		
	// Configure Serial Port as 8-N-1
	// (8 data bits, No parity and 1 stop bit)
	UART0->C1  = 0x00;
	
	// Configure Tx/Rx Interrupts
	UART0->C2  |= UART_C2_TIE(0);  // Tx Interrupt disabled
	UART0->C2  |= UART_C2_TCIE(0); // Tx Complete Interrupt disabled
	UART0->C2  |= UART_C2_RIE(1);    // Rx Interrupt enabled
 
	// Configure Transmitter/Receiever
	UART0->C2  |= UART_C2_TE(1);     // Tx Enabled
	UART0->C2  |= UART_C2_RE(1);     // Rx Enabled
	
	has_byte = 0;
	tx_active = 0;
	
	NVIC_SetPriority(UART0_IRQn, 128); // 0, 64, 128 or 192
	NVIC_ClearPendingIRQ(UART0_IRQn); 
	NVIC_EnableIRQ(UART0_IRQn);
	
	//UART0->C2 |= UART_C2_RIE_MASK;
	//UART0->C2 &= ~UART_C2_TIE_MASK;
	//UART0->C2 &= ~UART_C2_TCIE_MASK;
}

uint8_t Com_HasByte(void) {
	return has_byte;
}
uint8_t Com_GetByte(void) {
	has_byte = 0;
	return last_byte;
}

uint8_t Com_ReadReady(void) {
	if(rx_count == 0)
		return 1;
	return 0;
}
	
uint8_t Com_WriteReady(void) {
	if(tx_count == 0)
		return 1;
	return 0;
}

void Com_ReadByteArray(uint8_t* buffer, uint32_t length, void (*callback)(void)) {
    rx_buff = buffer;
	rx_count = length;
	rx_ind = 0;
	rx_call = callback;
}

void Com_WriteByteArray(uint8_t* buffer, uint32_t length, void (*callback)(void)) {
	tx_buff = buffer;
	tx_count = length;
	tx_ind = 0;
	tx_call = callback;
	tx_active = 1;
	
	while(!(UART0->S1 & UART_S1_TDRE_MASK));
	UART0->C2 |= UART_C2_TIE_MASK;
	UART0->D = tx_buff[0]; 
}

void UART0_IRQHandler(void) {
	NVIC_ClearPendingIRQ(UART0_IRQn);
	if (tx_active && (UART0->S1 & UART_S1_TDRE_MASK)) {
		// can send another character
		if(tx_count > 0) {
			tx_count--;
			UART0->D = tx_buff[tx_ind++];
		} else {
			UART0->C2 &= ~UART_C2_TIE_MASK;
			tx_active = 0;
			tx_call();
		}	
	} 
	if (UART0->S1 & UART_S1_RDRF_MASK) {
		if(rx_count  > 0) {
			rx_buff[rx_ind++] = UART0->D;
			rx_count--;
			if(rx_count == 0)
				rx_call();
		} else {
			last_byte = UART0->D;
			has_byte = 1;
		}
	} 
	if (UART0->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK)) {
			// handle the error
			// clear the flag
			UART0->S1 = UART_S1_OR_MASK | UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK;	
	}
}

char stdin_getchar(void) {
	while (!(UART0->S1 & UART_S1_RDRF_MASK));
	return UART0->D;
}

void stdout_putchar (char ch) {
	//Wait until transmit buffer is empty
	while(!(UART0->S1 & UART_S1_TDRE_MASK));
	//write to tx buffer
	UART0->D = ch;
}

void stdout_putstr(char* str, char max) {
	int i;
	for(i = 0; i < max; i++) {
		if(str[i] == 0)
			return;
		stdout_putchar(str[i]);
	}
}
