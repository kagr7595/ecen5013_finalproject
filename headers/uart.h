/***************************************************************************
*
*  	Filename: uart.h
*	Description: Header file for uart.c
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _UART_H
#define _UART_H

#ifdef FRDM
#include "MKL25Z4.h"
#include "core_cm0plus.h"
//#include "startup_MKL25Z4.S"
#endif

#include "circbuf.h"

/* Defines and Structures section
 ***************************************************************************/
#define BUF_LENGTH 100
#define OVER_S 16
#define BAUD 9600

/* Function prototype Section
 * Add protoypes for all functions called by this module, with the exception
 * of runtime routines.
 ***************************************************************************/

// UART Receive data to be stored into another pointer for processing
void uart_rx_data(uint8_t *read, uint8_t length);

// UART send data to uart
void uart_tx_data(uint8_t *send, uint8_t length);

// Sets up UART0 registers
void init_uart(void);

// IRQ handler routine for when a character is received
//void UART0_IRQHandler(void); //__attribute__((interrupt("IRQ")));

#endif
