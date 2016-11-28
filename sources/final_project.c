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

uint8_t final_project() {

#ifdef TSI_TEST
	RGB_init();
	tsi_init();
	uart_init();

	uint8_t num_correct = 0;

	uint8_t str_pass[5] = "PASS\n";
	uint8_t str_fail[5] = "FAIL\n";

	uint8_t str1[17] = "TSI UNIT TEST 1: ";
	LOG_0(str1, 17);
	uint8_t test1 = tsi_calibration_test();
	if(test1 == PASS){
		LOG_0(str_pass, 5);
		num_correct++;
	}
	else{
		LOG_0(str_fail, 5);
	}

	uint8_t str2[17] = "TSI UNIT TEST 2: ";
	LOG_0(str2, 17);
	uint8_t test2 = tsi_location_map_test();
	if(test2 == PASS){
		LOG_0(str_pass, 5);
		num_correct++;
	}
	else{
		LOG_0(str_fail, 5);
	}

	uint8_t str3[17] = "TSI UNIT TEST 3: ";
	LOG_0(str3, 17);
	uint8_t test3 = tsi_LED_control_test();
	if(test3 == PASS){
		LOG_0(str_pass, 5);
		num_correct++;
	}
	else{
		LOG_0(str_fail, 5);
	}

	uint8_t str_correct[15] = "TSI UNIT TESTS ";
	LOG_1(str_correct, 15, num_correct, UI8);
	uint8_t str_correct1[3] = "/3\n";
	LOG_0(str_correct1, 3);

#elif LCD_TEST
	lcd_init();

#else
	// Initialize all devices: Timer, RGB LED, UART(for now), LCD, Touch Slider, Peterson lock
	lcd_init();
	tsi_init();
	RGB_init();
	timer_init();
	uart_init();

	// Starts the game loop (infinite loop)
	game_run();

	return 0;
#endif
}

#endif
