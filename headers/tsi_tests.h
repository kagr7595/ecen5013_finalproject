/***************************************************************************
*
*  	Filename: tsi_tests.h
*	Description: Header file for tsi_tests.c
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _TSI_TESTS_H
#define _TSI_TESTS_H

/* Defines and Structures section
 ***************************************************************************/
#include <stdint.h>
#include "tsi.h"
#include "RGB.h"
#include "log.h"
#include "uart.h"

enum TEST {PASS, FAIL};

/* Function prototype Section
 * Add prototypes for all functions called by this module, with the exception
 * of runtime routines.
 ***************************************************************************/

// Checks the slider calibration by making sure no touch returns approximately 0s
uint8_t tsi_calibration_test();

// Tests to make sure each region of the slider can be mapped correctly into 4 regions
uint8_t tsi_location_map_test();

// Tests that the slider controls the LED color
uint8_t tsi_LED_control_test();

#endif
