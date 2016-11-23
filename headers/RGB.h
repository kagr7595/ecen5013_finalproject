/***************************************************************************
*
*  	Filename: RGB.h
*	Description: Header file for RGB.c
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _RGB_H
#define _RGB_H

#include "MKL25Z4.h"
#include "core_cm0plus.h"
#include <stdint.h>

/* Defines and Structures section
 ***************************************************************************/
// uses bit patterns to represent each of the three colors
// bit1 = BLUE
// bit2 = GREEN
// bit3 = RED
enum LED {OFF, BLUE, GREEN, BLUE_GREEN, RED, RED_BLUE, RED_GREEN, RGB};

/* Function prototype Section
 * Add protoypes for all functions called by this module, with the exception
 * of runtime routines.
 ***************************************************************************/
// Initializes the PWM and ports for controlling the RGB LED
void RGB_init();

// Change the LED brightness by changing the PWM of the input and change the colors
void set_PWM_RGB(uint8_t duty, uint8_t rgb);

#endif
