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
#define PRESCALAR 6
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
uint8_t overflow_target;

// Global variable to communicate a timer event to the game state machine
volatile uint8_t TIMER_EVENT = 0;

// Initialize the timer
void timer_init() {

    // Disable Interrupt for TPM1
    NVIC_DisableIRQ(TPM1_IRQn);

	// Initialize overflow count
	overflow_cnt = 0;

	// Clocking enabled to TPM1
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

    // Enable Interrupt for TPM1
    NVIC_EnableIRQ(TPM1_IRQn);

    __enable_irq();
}

// starts the timer over
// take input {speed_divider) this changes the number of timer overflows that must occur before resetting the timer
// SLOW -> overflow = 15 (3 sec), MEDIUM -> overlow = 10 (2 sec), FAST -> overflow = 5 (1 sec)
void timer_start(uint8_t speed_divider) {
	// Holds the LPTPM counter value, writing to COUNT clears the counter
	overflow_target = speed_divider;
	overflow_cnt = 0;
	TPM1_CNT = 0;
	NVIC_EnableIRQ(TPM1_IRQn);
}

// Converts the count pulled from the timer to a time value in terms of microseconds
uint64_t count_to_time(uint64_t count) {
	return (count*MICRO_CONV)/(CLOCK/64);
}

// Disables the timer interrupt
void timer_end() {
	// Disables the interrupt
    // Enable Interrupt for TPM1
    NVIC_DisableIRQ(TPM1_IRQn);
	return;
}

extern void TPM1_IRQHandler() {
    // Checks the Timer Overflow Flag, 1 indicates counter has overflowed
	if (TPM1_STATUS & TPM_STATUS_TOF_MASK) {
		overflow_cnt++;
		// clears the TOF flag
		TPM1_SC |= TPM_SC_TOF_MASK;
	}
	if (overflow_cnt == overflow_target){
		overflow_cnt = 0;
		TIMER_EVENT = 1;
	}
}
#endif
