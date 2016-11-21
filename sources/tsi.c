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

// stores the values read from the register
volatile uint16_t raw_tsi_val[2];
// stores the base values of the sensor to find the offset
volatile uint16_t base_tsi_val[2];

// Initializes the hardware for the TSI and reads in the base values for each of the two sensors
uint8_t tsi_init(){
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
			bin_location = LEFT;
		}
		else if(final_pos < 50){
			bin_location = CENTER_LEFT;
		}
		else if(final_pos < 75){
			bin_location = CENTER_RIGHT;
		}
		else{
			bin_location = RIGHT;
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

	// Start scan on opposite channel
	if(ch == 9){ch = 10;}
	else{ch = 9;}
	TSI0_DATA = tsi_start_scan(ch);
}

#endif
