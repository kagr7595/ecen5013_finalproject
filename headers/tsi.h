/***************************************************************************
*
*  	Filename: tsi.h
*	Description: Header file for tsi.c
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _TSI_H
#define _TSI_H

#include "MKL25Z4.h"
#include "core_cm0plus.h"
#include <stdint.h>
#include "RGB.h"
#include "LED_control.h"
#include "gameSS.h"

/* Defines and Structures section
 ***************************************************************************/
// Sets the channel to be read and starts a software trigger for the TSI
#define tsi_start_scan(ch) (TSI_DATA_TSICH(ch) | TSI_DATA_SWTS_MASK)
#define tsi_scan_done (TSI0_GENCS & TSI_GENCS_EOSF_MASK)
#define TSI_COUNT (TSI0_DATA & TSI_DATA_TSICNT_MASK)

enum POSITION {NONE, RIGHT, LEFT, CENTER_RIGHT, CENTER_LEFT};

/* Function prototype Section
 * Add prototypes for all functions called by this module, with the exception
 * of runtime routines.
 ***************************************************************************/

// This function translates between the electrode values and the RGB LED colors
// The first input corresponds to the first electrode reading (connected to TSI9)
// and the second input corresponds to the second electrode reading (connected to TSI10)
uint8_t bin_to_color(uint8_t loc_bin);

// Initializes the registers to start the TSI and connect the device pins
void tsi_init();

// Reads the touch sensor output
uint16_t tsi_check(uint8_t ch);

// Calculates the position on the slider binned to NONE, RIGHT, LEFT, CENTER_RIGHT, CENTER_LEFT
uint8_t tsi_position(uint16_t elec0, uint16_t elec1);

#endif
