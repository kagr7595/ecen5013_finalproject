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

/* Function prototype Section
 * Add protoypes for all functions called by this module, with the exception
 * of runtime routines.
 ***************************************************************************/
// Initialize the timer
void init_timer(void);

// Converts the count to number of microseconds
uint64_t count_to_time(uint64_t count);

// Resets the timer value
void timer_start(void);

// Retrieves the timer value
uint64_t timer_end(void);

#endif
