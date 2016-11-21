/***************************************************************************
*
*  	Filename: gameSS.h
*	Description: Header file for gameSS.c
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _GAMESS_H
#define _GAMESS_H

/* Defines and Structures section
 ***************************************************************************/
#include "RGB.h"
#include "LED_Control.h"
#include "tsi.h"
#include <stdint.h>

// Defines the game states
enum GAME_STATES {START, WAIT, NEW_COLOR, END};

/* Function prototype Section
 * Add prototypes for all functions called by this module, with the exception
 * of runtime routines.
 ***************************************************************************/
// This function translates between the electrode values and the RGB LED colors
// The first input corresponds to the first electrode reading (connected to TSI9)
// and the second input corresponds to the second electrode reading (connected to TSI10)
uint8_t bin_to_color(uint8_t loc_bin);

uint8_t game_start();

uint8_t game_request_color();

uint8_t game_end();

#endif
