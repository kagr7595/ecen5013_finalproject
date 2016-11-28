/***************************************************************************
*
*  	Filename: tsi.c
*	Description: Initializes the hardware for the lcd
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _LCD_C
#define _LCD_C

#include "lcd.h"

// Initializes the hardware for the TSI and reads in the base values for each of the two sensors
// RGB_init() must be called prior
void lcd_init(){
    // Enable clock gating for PORTC (Using PORTC inputs/outputs for LCD)
    SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;

    // Sets mux to Alternative 1 to use as GPIO
    PORTC_PCR0 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //Read/Write (Read is Active High)
    PORTC_PCR10 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //RS: Command Register (Active Low)/Write Register (Active High)
    PORTC_PCR11 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //Enable
    PORTC_PCR3 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //D0 (LSB)
    PORTC_PCR4 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //D1
    PORTC_PCR5 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //D2
    PORTC_PCR6 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //D3
    PORTC_PCR12 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //D4
    PORTC_PCR13 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //D5
    PORTC_PCR16 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //D6
    PORTC_PCR17 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //D7 (HSB)

    //Setting the pins as outputs (PDDR = Port Data Direction Register)
    FGPIOC_PDDR = LCD_RW;
    FGPIOC_PDDR = LCD_RS;
    FGPIOC_PDDR = LCD_EN;
    //The below data pins need to be able to be changed to input when doing a read or to know if the lcd is busy (LCD_D7)
    //Currently will set as output
    FGPIOC_PDDR = (LCD_D7 | LCD_D6 | LCD_D5 | LCD_D4 | LCD_D3 | LCD_D2 | LCD_D1 | LCD_D0);

    //Get into 8bit mode
    lcd_8bit_mode();

    //8bit interface, 2 lines, 5x8 dots
    lcd_command_write(FUNCTION_SET,LOW,HIGH,HIGH);
    //Increment automatically, no display shift
    lcd_command_write(ENTRY_MODE_SET,LOW,HIGH,NA);
    //Turn display on, cursor on, no blinking
    lcd_command_write(DISPLAY_CTL,BLINK,HIGH,HIGH);
    //Clear display, set cursor position to zero
    lcd_command_write(CLEAR_DISPLAY,NA,NA,NA);

}

void lcd_command_write(uint8_t command, uint8_t param0, uint8_t param1, uint8_t param2)
{
	lcd_data ldat;


	if(command == CLEAR_DISPLAY)
	{
	    //data set as output
	    FGPIOC_PDDR = (LCD_D7 | LCD_D6 | LCD_D5 | LCD_D4 | LCD_D3 | LCD_D2 | LCD_D1 | LCD_D0);

		//Clear display - Clears display and returns cursor to the home position (address 0)
		GPIOC_PTOR &= ~(LCD_RS | LCD_RW | LCD_D7 | LCD_D6 | LCD_D5 | LCD_D4 | LCD_D3 | LCD_D2 | LCD_D1);
		GPIOC_PTOR = LCD_D2;
	}
	else if(command == CURSOR_HOME)
	{
	    //data set as output
	    FGPIOC_PDDR = (LCD_D7 | LCD_D6 | LCD_D5 | LCD_D4 | LCD_D3 | LCD_D2 | LCD_D1 | LCD_D0);

		//Cursor home - Returns cursor to home position.  ALso returns display being shifted to the original position.  DDRAM content remains unchanged
		GPIOC_PTOR &= ~(LCD_RS | LCD_RW | LCD_D7 | LCD_D6 | LCD_D5 | LCD_D4 | LCD_D3 | LCD_D2);
		GPIOC_PTOR = LCD_D1;
	}
	else if(command == ENTRY_MODE_SET)
	{
	    //data set as output
	    FGPIOC_PDDR = (LCD_D7 | LCD_D6 | LCD_D5 | LCD_D4 | LCD_D3 | LCD_D2 | LCD_D1 | LCD_D0);

		GPIOC_PTOR &= ~(LCD_RS | LCD_RW | LCD_D7 | LCD_D6 | LCD_D5 | LCD_D4 | LCD_D3);
		GPIOC_PTOR = LCD_D2;
		if(param0)
			lcd_write_reg(SHIFT_RW, HIGH);
		else
			lcd_write_reg(SHIFT_RW, LOW);

		if(param1)
			lcd_write_reg(INCREMENT, HIGH);
		else
			lcd_write_reg(INCREMENT, LOW);

	}
	else if(command == DISPLAY_CTL)
	{
	    //data set as output
	    FGPIOC_PDDR = (LCD_D7 | LCD_D6 | LCD_D5 | LCD_D4 | LCD_D3 | LCD_D2 | LCD_D1 | LCD_D0);

		GPIOC_PTOR &= ~(LCD_RS | LCD_RW | LCD_D7 | LCD_D6 | LCD_D5 | LCD_D4);
		GPIOC_PTOR = LCD_D3;
		if(param0)
			lcd_write_reg(BLINK, HIGH);
		else
			lcd_write_reg(BLINK, LOW);

		if(param1)
			lcd_write_reg(CURSOR, HIGH);
		else
			lcd_write_reg(CURSOR, LOW);

		if(param2)
			lcd_write_reg(DISPLAY, HIGH);
		else
			lcd_write_reg(DISPLAY, LOW);

	}
	else if(command == CURSOR_DISPLAY_SHIFT)
	{
	    //data set as output
	    FGPIOC_PDDR = (LCD_D7 | LCD_D6 | LCD_D5 | LCD_D4 | LCD_D3 | LCD_D2 | LCD_D1 | LCD_D0);

		GPIOC_PTOR &= ~(LCD_RS | LCD_RW | LCD_D7 | LCD_D6 | LCD_D5);
		GPIOC_PTOR = LCD_D4;
		if(param0)
			lcd_write_reg(SHIFT_DIR, HIGH);
		else
			lcd_write_reg(SHIFT_DIR, LOW);

		if(param1)
			lcd_write_reg(SHIFT_DISPLAY, HIGH);
		else
			lcd_write_reg(SHIFT_DISPLAY, LOW);

	}
	else if(command == FUNCTION_SET)
	{
	    //data set as output
	    FGPIOC_PDDR = (LCD_D7 | LCD_D6 | LCD_D5 | LCD_D4 | LCD_D3 | LCD_D2 | LCD_D1 | LCD_D0);

		GPIOC_PTOR &= ~(LCD_RS | LCD_RW | LCD_D7 | LCD_D6);
		GPIOC_PTOR = LCD_D5;
		if(param0)
			lcd_write_reg(FUNCTION_SET, HIGH);
		else
			lcd_write_reg(FUNCTION_SET, LOW);

		if(param1)
			lcd_write_reg(NUM_LINE, HIGH);
		else
			lcd_write_reg(NUM_LINE, LOW);

		if(param2)
			lcd_write_reg(DATA_LEN, HIGH);
		else
			lcd_write_reg(DATA_LEN, LOW);

	}
	else if(command == SET_CGRAM_ADDR)
	{
	    //data set as output
	    FGPIOC_PDDR = (LCD_D7 | LCD_D6 | LCD_D5 | LCD_D4 | LCD_D3 | LCD_D2 | LCD_D1 | LCD_D0);

		GPIOC_PTOR &= ~(LCD_RS | LCD_RW | LCD_D7);
		GPIOC_PTOR = LCD_D6;
		ldat.ldata = param0;
		if(ldat.ld7 || ldat.ld6)
		{
			//ERROR as address cannot be more than 6 bits
		}
		lcd_write_reg(D5,ldat.ld5);
		lcd_write_reg(D4,ldat.ld4);
		lcd_write_reg(D3,ldat.ld3);
		lcd_write_reg(D2,ldat.ld2);
		lcd_write_reg(D1,ldat.ld1);
		lcd_write_reg(D0,ldat.ld0);

	}
	else if(command == SET_DDRAM_ADDR)
	{
	    //data set as output
	    FGPIOC_PDDR = (LCD_D7 | LCD_D6 | LCD_D5 | LCD_D4 | LCD_D3 | LCD_D2 | LCD_D1 | LCD_D0);

		GPIOC_PTOR &= ~(LCD_RS | LCD_RW);
		GPIOC_PTOR = LCD_D7;
        ldat.ldata = param0;
		if(ldat.ld7)
		{
			//ERROR as address cannot be more than 7 bits
		}
		lcd_write_reg(D6,ldat.ld6);
		lcd_write_reg(D5,ldat.ld5);
		lcd_write_reg(D4,ldat.ld4);
		lcd_write_reg(D3,ldat.ld3);
		lcd_write_reg(D2,ldat.ld2);
		lcd_write_reg(D1,ldat.ld1);
		lcd_write_reg(D0,ldat.ld0);
	}
	else if(command == READ_BUSY_N_ADDR_CTR)
	{
	    //SET data as input
	    FGPIOC_PDDR &= ~(LCD_D7 | LCD_D6 | LCD_D5 | LCD_D4 | LCD_D3 | LCD_D2 | LCD_D1 | LCD_D0);

		GPIOC_PTOR &= ~(LCD_RS);
		GPIOC_PTOR = LCD_RW;

	}
	else if(command == WRITE)
	{
	    //data set as output
	    FGPIOC_PDDR = (LCD_D7 | LCD_D6 | LCD_D5 | LCD_D4 | LCD_D3 | LCD_D2 | LCD_D1 | LCD_D0);

		GPIOC_PTOR &= ~(LCD_RW);
		GPIOC_PTOR = LCD_RS;
		ldat.ldata = param0;
		lcd_write_reg(D7,ldat.ld7);
		lcd_write_reg(D6,ldat.ld6);
		lcd_write_reg(D5,ldat.ld5);
		lcd_write_reg(D4,ldat.ld4);
		lcd_write_reg(D3,ldat.ld3);
		lcd_write_reg(D2,ldat.ld2);
		lcd_write_reg(D1,ldat.ld1);
		lcd_write_reg(D0,ldat.ld0);
	}
	else if(command == READ)
	{
	    //SET data as input
	    FGPIOC_PDDR &= ~(LCD_D7 | LCD_D6 | LCD_D5 | LCD_D4 | LCD_D3 | LCD_D2 | LCD_D1 | LCD_D0);

		GPIOC_PTOR = (LCD_RS | LCD_RW);
	}
	else
	{
		//ERROR (not a command that is expected)
	}

	lcd_toggle_en(1);
}

void lcd_8bit_mode()
{
	//Mode selection - the lcd can start in 3 different states - get the mode to 8bit by repeating the following 3 times
	//Please see https://en.wikipedia.org/wiki/Hitachi_HD44780_LCD_controller for Mode Selection reasoning
	int i;
	for(i = 0; i<3; i++)
	{
		lcd_write_reg(D7, LOW);
		lcd_write_reg(D6, LOW);
		lcd_write_reg(D5, HIGH);
		lcd_write_reg(D4, HIGH);
		lcd_toggle_en(1);
	}

}

void lcd_toggle_en(uint8_t num_toggles)
{
	int i,j;
	for (i = 0; i<num_toggles; i++)
	{
		lcd_write_reg(EN,HIGH);
		//pause to allow LCD to accept/fetch data
		for(j = 0; j<255; j++) {};
		lcd_write_reg(EN,LOW);
	}
}

void lcd_data_write(uint8_t * character_string, uint8_t length)
{
	int i;
	if(length>32){};//ERROR will occur as the lcd screen only has 32 spots
	for (i = 0; i<length; i++)
	{
		lcd_command_write(WRITE,lcd_character_map(*(character_string+i)),NA,NA);
	}

	//move cursor back to the beginning
	lcd_command_write(CURSOR_HOME,NA,NA,NA);
}

void lcd_write_reg(uint8_t bit_name, uint8_t high)
{
	if(bit_name == RW)
	{
		if (high) { GPIOC_PTOR = LCD_RW; }
		else { GPIOC_PTOR &= ~(LCD_RW); }
	}


	else if(bit_name == RS)
	{
		if (high) { GPIOC_PTOR = LCD_RS; }
		else { GPIOC_PTOR &= ~(LCD_RS); }
	}


	else if(bit_name == EN)
	{
		if (high) { GPIOC_PTOR = LCD_EN; }
		else { GPIOC_PTOR &= ~(LCD_EN); }
	}


	else if((bit_name == D7) || (bit_name == BUSY_FLAG))
	{
		if (high) { GPIOC_PTOR = LCD_D7; }
		else { GPIOC_PTOR &= ~(LCD_D7); }
	}


	else if(bit_name == D6)
	{
		if (high) { GPIOC_PTOR = LCD_D6; }
		else { GPIOC_PTOR &= ~(LCD_D6); }
	}


	else if(bit_name == D5)
	{
		if (high) { GPIOC_PTOR = LCD_D5; }
		else { GPIOC_PTOR &= ~(LCD_D5); }
	}


	else if((bit_name == D4) || (bit_name == DATA_LEN))
	{
		if (high) { GPIOC_PTOR = LCD_D4; }
		else { GPIOC_PTOR &= ~(LCD_D4); }
	}


	else if((bit_name == D3) || (bit_name == NUM_LINE) || (bit_name == SHIFT_DISPLAY))
	{
		if (high) { GPIOC_PTOR = LCD_D3; }
		else { GPIOC_PTOR &= ~(LCD_D3); }
	}


	else if((bit_name == D2) || (bit_name == FONT) || (bit_name == SHIFT_DIR) || (bit_name == DISPLAY))
	{
		if (high) { GPIOC_PTOR = LCD_D2; }
		else { GPIOC_PTOR &= ~(LCD_D2); }
	}


	else if((bit_name == D1) || (bit_name == CURSOR) || (bit_name == INCREMENT))
	{
		if (high) { GPIOC_PTOR = LCD_D1; }
		else { GPIOC_PTOR &= ~(LCD_D1); }
	}


	else if((bit_name == D0) || (bit_name == BLINK) || (bit_name == SHIFT_RW))
	{
		if (high) { GPIOC_PTOR = LCD_D0; }
		else { GPIOC_PTOR &= ~(LCD_D0); }
	}

	else
	{
		//ERROR (not one of the possible enums)
	}
}

//input character, outputs 8 bit data parameter for lcd register
//only creating basic alphabet, numbers plus :,!.'*()+/\

uint8_t lcd_character_map(uint8_t character)
{
	if(character == '0') { return 0x30; }
	else if (character == '1') { return 0x31; }
	else if (character == '2') { return 0x32; }
	else if (character == '3') { return 0x33; }
	else if (character == '4') { return 0x34; }
	else if (character == '5') { return 0x35; }
	else if (character == '6') { return 0x36; }
	else if (character == '7') { return 0x37; }
	else if (character == '8') { return 0x38; }
	else if (character == '9') { return 0x39; }
	else if (character == 'A') { return 0x41; }
	else if (character == 'B') { return 0x42; }
	else if (character == 'C') { return 0x43; }
	else if (character == 'D') { return 0x44; }
	else if (character == 'E') { return 0x45; }
	else if (character == 'F') { return 0x46; }
	else if (character == 'G') { return 0x47; }
	else if (character == 'H') { return 0x48; }
	else if (character == 'I') { return 0x49; }
	else if (character == 'J') { return 0x4A; }
	else if (character == 'K') { return 0x4B; }
	else if (character == 'L') { return 0x4C; }
	else if (character == 'M') { return 0x4D; }
	else if (character == 'N') { return 0x4E; }
	else if (character == 'O') { return 0x4F; }
	else if (character == 'P') { return 0x50; }
	else if (character == 'Q') { return 0x51; }
	else if (character == 'R') { return 0x52; }
	else if (character == 'S') { return 0x53; }
	else if (character == 'T') { return 0x54; }
	else if (character == 'U') { return 0x55; }
	else if (character == 'V') { return 0x56; }
	else if (character == 'W') { return 0x57; }
	else if (character == 'X') { return 0x58; }
	else if (character == 'Y') { return 0x59; }
	else if (character == 'Z') { return 0x5A; }
	else if (character == 'a') { return 0x61; }
	else if (character == 'b') { return 0x62; }
	else if (character == 'c') { return 0x63; }
	else if (character == 'd') { return 0x64; }
	else if (character == 'e') { return 0x65; }
	else if (character == 'f') { return 0x66; }
	else if (character == 'g') { return 0x67; }
	else if (character == 'h') { return 0x68; }
	else if (character == 'i') { return 0x69; }
	else if (character == 'j') { return 0x6A; }
	else if (character == 'k') { return 0x6B; }
	else if (character == 'l') { return 0x6C; }
	else if (character == 'm') { return 0x6D; }
	else if (character == 'n') { return 0x6E; }
	else if (character == 'o') { return 0x6F; }
	else if (character == 'p') { return 0x70; }
	else if (character == 'q') { return 0x71; }
	else if (character == 'r') { return 0x72; }
	else if (character == 's') { return 0x73; }
	else if (character == 't') { return 0x74; }
	else if (character == 'u') { return 0x75; }
	else if (character == 'v') { return 0x76; }
	else if (character == 'w') { return 0x77; }
	else if (character == 'x') { return 0x78; }
	else if (character == 'y') { return 0x79; }
	else if (character == 'z') { return 0x7A; }
	else if (character == '!') { return 0x21; }
	else if (character == '?') { return 0x3F; }
	else if (character == '*') { return 0x2A; }
	else if (character == ':') { return 0x3A; }
	else if (character == '=') { return 0x3D; }
	else if (character == '(') { return 0x28; }
	else if (character == ')') { return 0x29; }
	else if (character == '.') { return 0x2E; }
	else if (character == ' ') { return 0x00; }
	else { return 0xEF; } //ERROR OUTPUT
}

#endif
