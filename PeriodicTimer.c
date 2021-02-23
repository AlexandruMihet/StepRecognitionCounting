#include <MKL25Z4.h>
#include "PeriodicTimer.h"

volatile char timerEvent;

void timer0_init(uint32_t interval) {
	//Enable clock signal to PIT
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
	//Stop PIT for configuration
	PIT_MCR = PIT_MCR_MDIS_MASK;
	//Set countdown time. Runs at bus clock, FLL/2. 1s at 24 MHz
	PIT_LDVAL0 = interval;
	//Channel 0, enable interrupt TIE, and enable channel TEN
	PIT_TCTRL_REG(PIT, 0) |= PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK;
	//Start PIT MDIS is set to 0 and FRZ to 1, disable on debug	
	PIT_MCR = PIT_MCR_FRZ_MASK;
	
	//CMSIS enable interrupt for PIT
	NVIC_EnableIRQ(PIT_IRQn);	
	timerEvent = 0;
}

//Standard CMSIS interrupt handler name for PIT
void PIT_IRQHandler(void) {
	//CMSIS reset interrup request
	NVIC_ClearPendingIRQ(PIT_IRQn);
	//Check if channel 0 generated the interrupt
	if( PIT_TFLG_REG(PIT,0) ) {
		//reset flag by writing 1 to it
		PIT_TFLG_REG(PIT,0) = PIT_TFLG_TIF_MASK;
		//set the flag to process the main loop
		timerEvent = 1;
	}
}

void config_sys_clock(void)
{
    // MCG_C1: CLKS (bit 7-6) = 00
    MCG->C1 |= MCG_C1_CLKS(0);  // Select PLL/FLL as clock source
     
    // MCG_C1: IREFS (bit 2)  = 1
    MCG->C1 |= MCG_C1_IREFS(1); // Select Inernal Reference clock
                                // source for FLL
     
    // MCG_C4: DRST_DRS (bit 6-5) = 01
    MCG->C4 |= MCG_C4_DRST_DRS(1); // Select DCO range as Mid range
                                   // DCO generates the FLL Clock
     
    // MCG_C4: DMX32 (bit 7) = 1
    MCG->C4 |= MCG_C4_DMX32(1);    // Select DCO frequency as 48Mhz
}
