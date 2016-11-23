/***************************************************************************
*
*  	Filename: tsi_tests.c
*	Description: Runs the unit tests for the TSI sensor
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _TSI_TESTS_C
#define _TSI_TESTS_C

#include "tsi_tests.h"

// Checks the slider calibration by making sure no touch returns approximately 0s
uint8_t tsi_calibration_test(){
	uint8_t i;
	for(i = 0; i < 20; i++){
		uint8_t val0 = tsi_check(0);
		uint8_t val1 = tsi_check(1);
		if((val0 > 10) || (val1 > 10)){
			return FAIL;
		}
	}
	return PASS;
}

// Tests to make sure each region of the slider can be mapped correctly into 4 regions
uint8_t tsi_location_map_test(){
	uint8_t temp[1];
	uint16_t elec0;
	uint16_t elec1;
	uint8_t loc_bin;

	uint8_t str_left[46] = "Press the left region, then type a character.\n";
	LOG_0(str_left, 46);
	uart_rx_data(temp,1);
	elec0 = tsi_check(0);
	elec1 = tsi_check(1);
	loc_bin = tsi_position(elec0, elec1);
	if(loc_bin != LEFT){
		return FAIL;
	}

	uint8_t str_center_left[53] = "Press the left-center region, then type a character.\n";
	LOG_0(str_center_left, 53);
	uart_rx_data(temp,1);
	elec0 = tsi_check(0);
	elec1 = tsi_check(1);
	loc_bin = tsi_position(elec0, elec1);
	if(loc_bin != CENTER_LEFT){
		return FAIL;
	}

	uint8_t str_center_right[54] = "Press the right-center region, then type a character.\n";
	LOG_0(str_center_right, 54);
	uart_rx_data(temp,1);
	elec0 = tsi_check(0);
	elec1 = tsi_check(1);
	loc_bin = tsi_position(elec0, elec1);
	if(loc_bin != CENTER_RIGHT){
		return FAIL;
	}

	uint8_t str_right[47] = "Press the right region, then type a character.\n";
	LOG_0(str_right, 47);
	uart_rx_data(temp,1);
	elec0 = tsi_check(0);
	elec1 = tsi_check(1);
	loc_bin = tsi_position(elec0, elec1);
	if(loc_bin != RIGHT){
		return FAIL;
	}

	return PASS;
}

// Tests that the slider controls the LED color
uint8_t tsi_LED_control_test(){
	uint8_t temp[1];
	uint16_t elec0;
	uint16_t elec1;
	uint8_t loc_bin;
	uint8_t touch_color;

	uint8_t str_left[46] = "Press the left region, then type a character.\n";
	LOG_0(str_left, 46);
	uart_rx_data(temp,1);
	elec0 = tsi_check(0);
	elec1 = tsi_check(1);
	loc_bin = tsi_position(elec0, elec1);
	touch_color = bin_to_color(loc_bin);
	if(touch_color != RGB){
		return FAIL;
	}

	uint8_t str_center_left[53] = "Press the left-center region, then type a character.\n";
	LOG_0(str_center_left, 53);
	uart_rx_data(temp,1);
	elec0 = tsi_check(0);
	elec1 = tsi_check(1);
	loc_bin = tsi_position(elec0, elec1);
	touch_color = bin_to_color(loc_bin);
	if(touch_color != RED){
		return FAIL;
	}

	uint8_t str_center_right[54] = "Press the right-center region, then type a character.\n";
	LOG_0(str_center_right, 54);
	uart_rx_data(temp,1);
	elec0 = tsi_check(0);
	elec1 = tsi_check(1);
	loc_bin = tsi_position(elec0, elec1);
	touch_color = bin_to_color(loc_bin);
	if(touch_color != BLUE){
		return FAIL;
	}

	uint8_t str_right[47] = "Press the right region, then type a character.\n";
	LOG_0(str_right, 47);
	uart_rx_data(temp,1);
	elec0 = tsi_check(0);
	elec1 = tsi_check(1);
	loc_bin = tsi_position(elec0, elec1);
	touch_color = bin_to_color(loc_bin);
	if(touch_color != GREEN){
		return FAIL;
	}


	return PASS;
}

#endif
