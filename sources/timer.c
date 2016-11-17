/***************************************************************************
*
*  	Filename: timer.c
*	Description: Timer application for the timer profiling
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _TIMER_C
#define _TIMER_C

#include <stdint.h>
#include "timer.h"
#include "circbuf.h"

#define MICRO_CONV 1000000
#define CLOCK 48000000
#define PRESCALAR 0x0004
/*000 Divide by 1
  001 Divide by 2
  010 Divide by 4
  011 Divide by 8
  100 Divide by 16
  101 Divide by 32
  110 Divide by 64
  111 Divide by 128*/

volatile CircBuf_t * rx_buf;
uint64_t overflow_cnt;

// Initialize the timer
void init_timer() {
#ifdef FRDM
	// Initialize overflow count
	overflow_cnt = 0;

	// Clocking enabled to TPM0 and TPM2
	SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK;

	// Source is from MCGFLLCLK clock or MCGPLLCLK/2 clock
	// This chooses the FLL
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);

	// Timer Overflow Interrupt Enabled, up counting mode, prescalar divide by PRESCALAR
	TPM1_SC = TPM_SC_CMOD(1) | TPM_SC_PS(PRESCALAR) | TPM_SC_TOIE_MASK;

	// Modulo value for the LPTPM counter, sets TOF in TPMx_SC when this count is reached
	TPM1_MOD = 0xFFFF;

    // Clear pending interrupt
    NVIC_ClearPendingIRQ(TPM1_IRQn);

    // Enable Interrupt for UART0, interrupt 12 in vector table
    NVIC_EnableIRQ(TPM1_IRQn);

    __enable_irq();
#endif
}

// starts the timer over
void timer_start() {
#ifdef FRDM
	// Holds the LPTPM counter value, writing to COUNT clears the counter
	overflow_cnt = 0;
	TPM1_CNT = 0;
#endif
}

// Converts the count pulled from the timer to a time value in terms of microseconds
uint64_t count_to_time(uint64_t count) {
	return (count*MICRO_CONV)/(CLOCK/PRESCALAR);
}

// returns the ending time
uint64_t timer_end() {
#ifdef FRDM
	// Holds the LPTPM counter value, writing to COUNT clears the counter
	return ((0x0000FFFF * overflow_cnt) + TPM1_CNT);
#else
	return 0;
#endif
}

#ifdef FRDM
extern void TPM1_IRQHandler() {
    // Checks the Timer Overflow Flag, 1 indicates counter has overflowed
	if (TPM1_STATUS & TPM_STATUS_TOF_MASK) {
		overflow_cnt++;
		// clears the TOF flag
		TPM1_SC |= TPM_SC_TOF_MASK;
	}
}
#endif

#endif
