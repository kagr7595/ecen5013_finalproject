/***************************************************************************
*
*  	Filename: LED_control.c
*	Description: High level functions to alter the color and brightness of the
*	LEDs
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _LED_CONTROL_C
#define _LED_CONTROL_C

#include "LED_control.h"

//global variables allow for the code to retain memory of what the LED was last set to
uint8_t color = OFF;
uint8_t duty = 50;

void set_brightness(uint8_t new_duty){
	// function runs directly if invoked by the FRDM board
	set_PWM_RGB(new_duty, color);
	duty = new_duty;
}

void set_color(uint8_t new_color){
	// function runs directly if invoked by the FRDM board
	set_PWM_RGB(duty, new_color);
	color = new_color;
}

#endif
