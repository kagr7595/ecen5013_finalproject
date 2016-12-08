/***************************************************************************
*
*  	Filename: LED_control.h
*	Description: Header file for LED_control.c
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _LED_CONTROL_H
#define _LED_CONTROL_H

#include "RGB.h"

void set_brightness(uint8_t new_duty);

void set_color(uint8_t new_color);

#endif
