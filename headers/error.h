/***************************************************************************
*
*  	Filename: error.h
*	Description: Header file for error.c
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _ERROR_H
#define _ERROR_H

#include <stdint.h>
#include <stdio.h>

/* Defines section
 * Add all #defines here
 ***************************************************************************/
#define MY_MEMMOVE 1
#define MY_MEMZERO 2
#define MY_REVERSE 3
#define MY_ITOA    4
#define MY_FTOA    5
#define LOG1_F     6
#define LOG2_F     6

// Prints out the error that occurred in each function 
void return_code_error(int8_t return_code, uint8_t function_name);

#endif
