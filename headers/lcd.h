/***************************************************************************
*
*  	Filename: lcd.h
*	Description: Header file for lcd.c
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _LCD_H
#define _LCD_H

#include "MKL25Z4.h"
#include "core_cm0plus.h"
#include <stdint.h>
#include "gameSS.h"

/* Defines and Structures section
 ***************************************************************************/
// Set pins for PORTC
#define LCD_RW (1<<7)
#define LCD_RS (1<<11)
#define LCD_EN (1<<9)
#define LCD_D0 (1<<3)
#define LCD_D1 (1<<4)
#define LCD_D2 (1<<5)
#define LCD_D3 (1<<6)
#define LCD_D4 (1<<12)
#define LCD_D5 (1<<13)
#define LCD_D6 (1<<16)
#define LCD_D7 (1<<17)

#define HIGH 1
#define LOW 0
#define NA 15

typedef union {
	struct {
		uint8_t ld0 : 1;
		uint8_t ld1 : 1;
		uint8_t ld2 : 1;
		uint8_t ld3 : 1;
		uint8_t ld4 : 1;
		uint8_t ld5 : 1;
		uint8_t ld6 : 1;
		uint8_t ld7 : 1;
	};
	uint8_t ldata;
} lcd_data;
enum LCD_BIT {RW, RS, EN, D7, D6, D5, D4, D3, D2, D1, D0, SHIFT_RW, BLINK, INCREMENT, CURSOR, DISPLAY, SHIFT_DIR, SHIFT_DISPLAY, FONT, NUM_LINE, DATA_LEN, BUSY_FLAG};

enum LCD_COMMAND {CLEAR_DISPLAY, CURSOR_HOME, ENTRY_MODE_SET, DISPLAY_CTL, CURSOR_DISPLAY_SHIFT, FUNCTION_SET, SET_CGRAM_ADDR, SET_DDRAM_ADDR, READ_BUSY_N_ADDR_CTR, WRITE, READ};

enum LCD_DELAY_SPEED {SHORT, MEDIUM, LONG};

typedef union {
	struct {
		uint32_t l00 : 1;
		uint32_t l01 : 1;
		uint32_t l02 : 1;
		uint32_t lD0 : 1;
		uint32_t lD1 : 1;
		uint32_t lD2 : 1;
		uint32_t lD3 : 1;
		uint32_t lRW : 1;
		uint32_t l08 : 1;
		uint32_t lEN : 1;
		uint32_t l0a : 1;
		uint32_t lRS : 1;
		uint32_t lD4 : 1;
		uint32_t lD5 : 1;
		uint32_t l0e : 1;
		uint32_t l0f : 1;
		uint32_t lD6 : 1;
		uint32_t lD7 : 1;
		uint32_t l12 : 1;
		uint32_t l13 : 1;
		uint32_t l14 : 1;
		uint32_t l15 : 1;
		uint32_t l16 : 1;
		uint32_t l17 : 1;
		uint32_t l18 : 1;
		uint32_t l19 : 1;
		uint32_t l1a : 1;
		uint32_t l1b : 1;
		uint32_t l1c : 1;
		uint32_t l1d : 1;
		uint32_t l1e : 1;
		uint32_t l1f : 1;
	};
	uint32_t lreg;
} lcd_reg;

/* Function prototype Section
 * Add prototypes for all functions called by this module, with the exception
 * of runtime routines.
 ***************************************************************************/

// Initializes the registers to start the LCD and connect the device pins
void lcd_init();

void lcd_write_reg(uint8_t bit_name, uint8_t high);

void lcd_command_write(uint8_t command, uint8_t param0, uint8_t param1, uint8_t param2);

void lcd_8bit_mode();

void lcd_toggle_en(uint8_t num_toggles);

void lcd_data_write(uint8_t * write_data, uint8_t length);

void lcd_data_write_append(uint8_t * write_data, uint8_t length);

void lcd_data_read(uint8_t * read_data, uint8_t length);

void lcd_display_en(uint8_t high);

void lcd_delay(uint8_t mode);

uint8_t lcd_character_map(uint8_t character);

void lcd_debug_log();

#endif
