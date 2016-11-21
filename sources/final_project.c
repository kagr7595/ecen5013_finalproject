/***************************************************************************
*
*  	Filename: final_project.c
*	Description: Initializes the hardware and initiates game play
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _FINAL_PROJECT_C
#define _FINAL_PROJECT_C

#include "final_project.h"

int final_project() {

	uart_init();
	tsi_init();
	RGB_init();

	uint16_t elec0;
	uint16_t elec1;
	uint8_t loc_bin;

	while(1){
		elec0 = tsi_check(0);
		elec1 = tsi_check(1);
		loc_bin = tsi_position(elec0, elec1);
		bin_to_color(loc_bin);
	}
	return 0;
}

#endif
