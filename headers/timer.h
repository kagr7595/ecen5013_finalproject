/***************************************************************************
*
*  	Filename: timer.h
*	Description: Header file for timer.c
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _TIMER_H
#define _TIMER_H

#include "MKL25Z4.h"
#include "core_cm0plus.h"
#include "LOG.h"

enum SPEED {SLOW = 15, FAST = 8};

/* Function prototype Section
 * Add protoypes for all functions called by this module, with the exception
 * of runtime routines.
 ***************************************************************************/
// Initialize the timer
void timer_init(void);

// Converts the count to number of microseconds
uint64_t count_to_time(uint64_t count);

// starts the timer over
// take input {speed_divider) this changes the number of timer overflows that must occur before resetting the timer
// SLOW -> overflow = 15 (3 sec), MEDIUM -> overlow = 10 (2 sec), FAST -> overflow = 5 (1 sec)
void timer_start(uint8_t speed_divider);

// Disables the timer interrupt
void timer_end(void);

#endif
