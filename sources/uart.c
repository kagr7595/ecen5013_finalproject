/***************************************************************************
*
*  	Filename: uart.c
*	Description: UART Application Layer code
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _UART_C
#define _UART_C

#include "uart.h"

volatile CircBuf_t * rx_buf;

// UART Receive data to be stored into another pointer for processing
void uart_rx_data(uint8_t *read, uint8_t length) {
	uint8_t item;
	uint8_t * item_ptr = &item;
	uint8_t i = 0;
	// Wait until RX buffer has a sufficient number of items
    while(rx_buf->num_items < length) {1;}

    while(i < length) {
        // Get character from RX buffer
    	NVIC_DisableIRQ(UART0_IRQn);
        remove_buffer_item(rx_buf, item_ptr);
        NVIC_EnableIRQ(UART0_IRQn);
        // Write character to read array
        *(read + i) = item;
        // Turn RX interrupt on
        UART0_C2 |= UART_C2_RIE_MASK;
        i++;
    }
}

// UART send data to uart
void uart_tx_data(uint8_t *send, uint8_t length) {
    uint8_t i;
    for (i = 0; i < length; i++) {
    	while(!(UART0_S1 & UART_S1_TDRE_MASK)) {;}
    	UART0_D = *(send + i);
    }
}

extern void UART0_IRQHandler() {
	uint8_t * item;
	uint8_t status = UART0_S1;

	// If the receive register is full and there is room in the RX buffer, read a character
	if ((status & UART_S1_RDRF_MASK) && (!buffer_full(rx_buf))) {
		NVIC_DisableIRQ(UART0_IRQn);
		add_buffer_item(rx_buf, UART0_D);
		NVIC_EnableIRQ(UART0_IRQn);
		// Only clear interrupt flag for receive if the RX buffer is full
		if(buffer_full(rx_buf)) {
			UART0_C2 &= ~UART_C2_RIE_MASK;
		}
	}
}

void init_uart() {
	__disable_irq();
	// Enable clock at portA
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
	// Enable clock gating to UART0
	SIM_SCGC4 |= SIM_SCGC4_UART0(1);

    // Source for UART0 is from MCGFLLCLK clock or MCGPLLCLK/2 clock
	// This chooses the FLL
	SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1) | SIM_SOPT2_PLLFLLSEL(0);

    // Sets mux to Alternative 2 to enable UART0 on pins to computer
    PORTA_PCR1 = PORT_PCR_MUX(2);
    PORTA_PCR2 = PORT_PCR_MUX(2);

	// UART set to 8 bit mode with no parity checking
	UART0_C1 = 0;
	// TX and RX disabled for now, change after settings
	UART0_C2 = 0;
	// no data inversion or extra interrupts
	UART0_C3 = 0;
	// no data inversion, LSB first, normal break char length, disable LIN break detect
	UART0_S2 = 0;

	// Set baud rate
	uint16_t baud_div = (DEFAULT_SYSTEM_CLOCK/(BAUD*OVER_S));

    // Baud Rate modulo divisor
    UART0_BDH |= (baud_div >> 8) & UART0_BDH_SBR_MASK;
    UART0_BDL = (baud_div) & UART0_BDL_SBR_MASK;

	// Set oversampling to 16
    UART0_C4 = UART0_C4_OSR(OVER_S - 1);

    // Initialize the RX buffers
    rx_buf = buffer_init(BUF_LENGTH);

    // Enable TX, RX, and RXI (receive interrupt)
    UART0_C2 = (UART_C2_RIE_MASK | UART0_C2_RE_MASK | UART0_C2_TE_MASK);

    // Clear pending interrupt
    NVIC_ClearPendingIRQ(UART0_IRQn);

    // Enable Interrupt for UART0, interrupt 12 in vector table
    NVIC_EnableIRQ(UART0_IRQn);

	__enable_irq();
}

#endif
