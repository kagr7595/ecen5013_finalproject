/***************************************************************************
*
*  	Filename: gameSS.c
*	Description: Contains functions that store the current game state and transition
*	between different states
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _GAMESS_C
#define _GAMESS_C

#include "gameSS.h"

// This function translates between the electrode values and the RGB LED colors
// The first input corresponds to the first electrode reading (connected to TSI9)
// and the second input corresponds to the second electrode reading (connected to TSI10)
uint8_t bin_to_color(uint8_t loc_bin){
	if(loc_bin == NONE){
		set_color(OFF);
	}
	else if(loc_bin == LEFT){
		set_color(RGB);
	}
	else if(loc_bin == CENTER_LEFT){
		set_color(RED);
	}
	else if(loc_bin == CENTER_RIGHT){
		set_color(BLUE);
	}
	else{
		set_color(GREEN);
	}
	return 0;
}

#endif
