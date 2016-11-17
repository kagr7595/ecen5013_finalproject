/***************************************************************************
*
*  	Filename: RGB.c
*	Description: Controls the initialization of the PWM controlled RGB LED and
*	             its control functions
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _RGB_C
#define _RGB_C

#include "RGB.h"

// Initializes the PWM and ports for controlling the RGB LED
void RGB_init() {
	// Clocking enabled to PortB and PortD
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;

	// Clocking enabled to TPM0 and TPM2
	SIM_SCGC6 |= (SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM2_MASK);

	// Source is from MCGFLLCLK clock or MCGPLLCLK/2 clock
	// This chooses the FLL
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);

	PORTB_PCR18 = PORT_PCR_MUX(3); //TPM2_CH0
	PORTB_PCR19 = PORT_PCR_MUX(3); //TPM2_CH1
	PORTD_PCR1 = PORT_PCR_MUX(4); //TPM0_CH1

	// Modulo value for the LPTPM counter, sets TOF in TPMx_SC when this count is reached
	// 100% duty cycle
	TPM0_MOD = 0xFFFF;
	TPM2_MOD = 0xFFFF;

	// Sets the configuration for the PWM signals, channel specific
	// Also has the CHF Channel Flag for HW event on channel, CHIE Channel IE
	TPM0_C1SC = TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK;
	TPM2_C0SC = TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK;
	TPM2_C1SC = TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK;

	// up counting mode, prescalar divide by 1
	TPM0_SC = TPM_SC_CMOD(1);
	TPM2_SC = TPM_SC_CMOD(1);

	// Contains channel value VAL in bits 15-0
	TPM0_C1V = 0xFFFF; //blue
	TPM2_C0V = 0xFFFF; //red
	TPM2_C1V = 0xFFFF; //green
}

// Change the LED brightness by changing the PWM of the input
// Takes a duty cycle in (0-100)
void set_PWM_RGB(uint8_t duty, uint8_t rgb) {
	if((duty <= 100) && (duty >= 0) && (rgb >= 0) && (rgb <= 7)) {
		// Order of multiplication to prevent the number from becoming a fraction -> 0
		TPM0_C1V = (duty*(0xFFFF/100))*(rgb & 0x01); //blue);
		TPM2_C0V = (duty*(0xFFFF/100))*((rgb & 0x04) >> 2); //red
		TPM2_C1V = (duty*(0xFFFF/100))*((rgb & 0x02) >> 1); //green
	}
}

#endif
