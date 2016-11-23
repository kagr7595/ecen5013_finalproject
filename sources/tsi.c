/***************************************************************************
*
*  	Filename: tsi.c
*	Description: Initializes the hardware for the touch sensor
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _TSI_C
#define _TSI_C

#include "tsi.h"

// calibration flag
volatile uint8_t cal_flag = 1;

// stores the values read from the register
volatile uint16_t raw_tsi_val[2];
// stores the base values of the sensor to find the offset
volatile uint16_t base_tsi_val[2];

// global variables meant to communicate with the game state machine
volatile uint8_t COLOR_EVENT = 0;
volatile uint8_t RELEASE_EVENT = 0;
volatile uint8_t touch_color = 0;

// This function translates between the electrode values and the RGB LED colors
// The first input corresponds to the first electrode reading (connected to TSI9)
// and the second input corresponds to the second electrode reading (connected to TSI10)
uint8_t bin_to_color(uint8_t loc_bin){
	if(loc_bin == LEFT){
		set_color(RGB);
		return RGB;
	}
	else if(loc_bin == CENTER_LEFT){
		set_color(RED);
		return RED;
	}
	else if(loc_bin == CENTER_RIGHT){
		set_color(BLUE);
		return BLUE;
	}
	else if(loc_bin == RIGHT){
		set_color(GREEN);
		return GREEN;
	}
	else{
		set_color(OFF);
		return OFF;
	}
}

// Initializes the hardware for the TSI and reads in the base values for each of the two sensors
// RGB_init() must be called prior
void tsi_init(){
	__disable_irq();

    // Enable clock gating for TSI
    SIM_SCGC5 |= SIM_SCGC5_TSI_MASK;
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;

    TSI0_GENCS |= (TSI_GENCS_ESOR_MASK  // End of Scan Interrupt Enabled
    		   | TSI_GENCS_MODE(0)
    		   | TSI_GENCS_REFCHRG(4)   // 8uA reference oscillator charge current value
			   | TSI_GENCS_DVOLT(0)     // Voltage Rails: DV = 1.03V, Vp = 1.33V, Vm = 0.30V
			   | TSI_GENCS_STPE_MASK
			   | TSI_GENCS_EXTCHRG(7)   // Electrode oscillator current value
			   | TSI_GENCS_PS(4)        // Prescaler for Electrode Oscillator Frequency (16)
			   | TSI_GENCS_NSCN(11)     // 12 scans per electrode
			   | TSI_GENCS_TSIIEN_MASK);// TSI interrupt enabled

    TSI0_GENCS |= TSI_GENCS_TSIEN_MASK; // Enables the TSI module

    // Sets mux to Alternative 0 to enable TSI0 on pins to computer
    PORTB_PCR16 = PORT_PCR_MUX(0);
    PORTB_PCR17 = PORT_PCR_MUX(0);

    __enable_irq();
    // Find the base values of the two capacitive sensors
    // Channel 0
    TSI0_DATA = tsi_start_scan(9);
    while(!tsi_scan_done){;}
    TSI0_GENCS |= TSI_GENCS_EOSF_MASK;
    *base_tsi_val = TSI_COUNT;

    // Channel 1
    TSI0_DATA = tsi_start_scan(10);
    while(!tsi_scan_done){;}
    TSI0_GENCS |= TSI_GENCS_EOSF_MASK;
    *(base_tsi_val+1) = TSI_COUNT;
    __disable_irq();

    cal_flag = 0;

    // Clear pending interrupt
    NVIC_ClearPendingIRQ(TSI0_IRQn);
    // Enable Interrupt for UART0, interrupt 12 in vector table
    NVIC_EnableIRQ(TSI0_IRQn);

	__enable_irq();

	// Start scanning
	TSI0_DATA = tsi_start_scan(9);
}

// Takes the difference between the currently stored raw data and the saved baseline data
uint16_t tsi_check(uint8_t ch){
	NVIC_DisableIRQ(TSI0_IRQn);
	if((*(raw_tsi_val+ch) - *(base_tsi_val+ch)) < 0){
		NVIC_EnableIRQ(TSI0_IRQn);
		return 0;
	}
	uint16_t value = (uint16_t)(*(raw_tsi_val+ch) - *(base_tsi_val+ch));
	NVIC_EnableIRQ(TSI0_IRQn);
	return value;
}

// Calculates the position on the slider binned to NONE, RIGHT, LEFT, CENTER_RIGHT, CENTER_LEFT
uint8_t tsi_position(uint16_t elec0, uint16_t elec1) {
	uint8_t pos0, pos1, final_pos;
	uint8_t bin_location = NONE;
	// only calculate if above threshold
	if((elec0 > 10) || (elec1 > 10)){
		// calculate position according to elec0 as a percentage
		pos0 = (100*elec0)/(elec0+elec1);
		// calculate position according to elec1 as a percentage
		// invert slider position to match pos0
		pos1 = 100-((100*elec1)/(elec0+elec1));
		// average the positions according to each node
		final_pos = (pos0+pos1)/2;
		// convert position to a bin
		if(final_pos < 25){
			bin_location = RIGHT;
		}
		else if(final_pos < 50){
			bin_location = CENTER_RIGHT;
		}
		else if(final_pos < 75){
			bin_location = CENTER_LEFT;
		}
		else{
			bin_location = LEFT;
		}
	}
	return bin_location;
}

extern void TSI0_IRQHandler() {
	// See which channel was just read
	uint8_t ch = (TSI0_DATA & TSI_DATA_TSICH_MASK) >> TSI_DATA_TSICH_SHIFT;
	// Read data value and store it
	TSI0_GENCS |= TSI_GENCS_EOSF_MASK;
	*(raw_tsi_val+ch-9) = TSI_COUNT;

	// only want to run LED when not running for calibration
	if(cal_flag == 0){
		// Set LED color and check if flag must be set for the COLOR_EVENT
		uint16_t elec0 = tsi_check(0);
		uint16_t elec1 = tsi_check(1);
		uint8_t loc_bin = tsi_position(elec0, elec1);
		touch_color = bin_to_color(loc_bin);

		if (loc_bin != NONE){
			COLOR_EVENT = 1;
		}
		else{
			RELEASE_EVENT = 1;
		}

		// Start scan on opposite channel
		if(ch == 9){ch = 10;}
		else{ch = 9;}
		TSI0_DATA = tsi_start_scan(ch);
	}
}

#endif
