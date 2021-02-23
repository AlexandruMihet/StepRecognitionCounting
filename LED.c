 #include <MKL25Z4.h>
 #include "LED.h"


void init_RGB(void){
	//Enable clock for port B and D
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;
	
	//Set mode to GPIO
	PORTB->PCR[RED_LED] &= ~(uint32_t)0x700;
	PORTB->PCR[GREEN_LED] &= ~(uint32_t)0x700;
	PORTD->PCR[BLUE_LED] &= ~(uint32_t)0x700;
	
	PORTB->PCR[RED_LED] |= 0x100;
	PORTB->PCR[GREEN_LED] |= 0x100;
	PORTD->PCR[BLUE_LED] |= 0x100;
	
	//Set outputs
	PTB->PDDR |= (1 << RED_LED) | (1 << GREEN_LED);
	PTD->PDDR |= (1 << BLUE_LED);
	
	//Turn on LEDs
	PTB->PCOR = ((uint32_t)1 << RED_LED) | ((uint32_t)1 << GREEN_LED);
	PTD->PCOR = ((uint32_t)1 << BLUE_LED);
}

void toggle(void){
	//Toggle the state of the LEDs
	PTB->PTOR = (1 << RED_LED);
	PTB->PTOR = (1 << GREEN_LED);
	PTD->PTOR = (1 << BLUE_LED);
}

void toggle_r(void){
	PTB->PTOR = (1 << RED_LED);
}

void toggle_g(void){
	PTB->PTOR = (1 << GREEN_LED);
}

void toggle_b(void){
	PTD->PTOR = (1 << BLUE_LED);
}

void set_r(char on_off) {
	if(on_off) {
		//Zero on pin turns on the led
		PTB->PCOR = (1 << RED_LED);
	} else {
		PTB->PSOR = (1 << RED_LED);
	}
}

void set_g(char on_off) {
	if(on_off) {
		//Zero on pin turns on the led
		PTB->PCOR = (1 << GREEN_LED);
	} else {
		PTB->PSOR = (1 << GREEN_LED);
	}
}

void set_b(char on_off) {
	if(on_off) {
		//Zero on pin turns on the led
		PTD->PCOR = (1 << BLUE_LED);
	} else {
		PTD->PSOR = (1 << BLUE_LED);
	}
}

