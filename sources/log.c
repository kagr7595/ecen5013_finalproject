/***************************************************************************
*
*  	Filename: log.c
*	Description: Logging capability code
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _LOG_C
#define _LOG_C

#include "uart.h"
#include "log.h"

// Logging to UART or to terminal with a simple string and length of characters in string
// void LOG_0(<type> * data, <type> len)
void LOG_0(uint8_t * data, uint8_t len)
{
#ifdef VERBOSE
	uart_tx_data(data,len);
#endif
}

//Logging a string with specific length of characters and appending a parameter
//void LOG_1(<type> * data, <type> len, <type> * param, <type> data_type_size)
void LOG_1(uint8_t * data, uint8_t len, uint64_t i_data,  uint8_t data_type)
{
    uint8_t return_code  = 0;
    uint8_t str[100];
    uint16_t num_elements = 0;
    uint64_t temp; 
    int8_t i8;
    int16_t i16;
    int32_t i32;
    int64_t i64;

    LOG_0(data,len);
    
    if(str == NULL) {return_code= 1;}   
    return_code_error(return_code, LOG1_F);
    
    temp = i_data;
    switch(data_type)
    {
    case I8:
	i8 = (int8_t) i_data;
	// if data is a negative number
	if(i8 < 0)
	{
	    temp = (uint64_t)(~i8+1);
	} else
	{
	    temp = (uint64_t)i8;
	}
	
	break;
    case I16:
	i16 = (int16_t) i_data;
	// if data is a negative number
	if(i16 < 0)
	{
	    temp = (uint64_t)(~i16+1);
	} else
	{
	    temp = (uint64_t)i16;
	}
	break;
	
    case I32:
	i32 = (int32_t) i_data;
	// if data is a negative number
	if(i32 < 0)
	{
	    temp = (uint64_t)(~i32+1);
	} else
	{
	    temp = (uint64_t)i32;
	}
	break;
    case I64:
	i64 = (int64_t) i_data;
	// if data is a negative number
	if(i64 < 0)
	{
	    temp = (uint64_t)(~i64+1);
	} else
	{
	    temp = (uint64_t)i64;
	}
	break;
    default:
	break;
	
    }
    uint8_t base = 10;
    
    //switch from numbers to ascii characters
    for (num_elements = 0; temp != 0; num_elements++)
    {
	//assign remainder into the str pointer
	if(temp%base < 10)
	{
	    *(str + num_elements) = temp%base + '0';
	} 
	temp = temp/base;
    }
    
    if(num_elements == 0){
    	*(str) = '0';
    	num_elements++;
    }

    // if i_data is negative, change the largest bit (sign bit) in str to 1
    if (i32 < 0 || i64 < 0 || i16 < 0 || i8 < 0)
    {
	*(str+num_elements) = '-';  
	num_elements++;
    }
    
    //reverse array of remainders to get ascii string of i_data without sign
    return_code = my_reverse(str, num_elements);
    if(return_code != 0){return_code= 3;}
    
    LOG_0(str,num_elements);
}


//Logging a string with specific length of characters and appending a parameter
//void LOG_2(<type> * data, <type> len, <type> * param, <type> data_type_size)
void LOG_2(uint8_t * data, uint8_t len, float f_data,  uint8_t data_type)
{
    uint8_t return_code  = 0;
    uint8_t str[100];
    uint16_t num_elements = 0;

    LOG_0(data,len);
    
    if(str == NULL) {return_code= 1;}   
    return_code_error(return_code, LOG2_F);
    if(data_type == FL32)
    {
	return_code = my_ftoa(str,f_data);
	return_code_error(return_code, MY_FTOA);
	num_elements = count2null(str);
    } 
    LOG_0(str,num_elements);
}

// Takes in an array of hex characters and a length, printing out the hex in ascii
void LOG_3(uint8_t * hex, uint8_t len)
{
	uint8_t ascii[2*len];
    hex_to_ascii(hex, ascii, len);
    LOG_0(ascii,2*len);
}

// Creates a new logger repeatable character (as many as num_character). 
// Up to 256 of the same character (useful for spacing and newlines)
void my_newcharacter(uint8_t character, uint8_t num_character)
{
    uint8_t print [256] = "";
    uint8_t i;
    for(i = 0;i < num_character;i++)
    {
	*(print+i) = character;
    }
    LOG_0(print,i);
}

// Converts an array of bytes to an array of ascii characters
void hex_to_ascii(uint8_t* hex, uint8_t* ascii, uint8_t hex_length)
{
	// The length of the ascii array should be twice that of the hex array since each
	// hex number is 4 bits and each ascii 8 bits
	int8_t num1, num2;
	int8_t i = 0;
	for(i = 0; i < hex_length; i++){
		num1 = *(hex+i) >> 4;
		num2 = *(hex+i) & 0x0F;
		if(num1 < 10){
			*(ascii+2*i) = num1 + '0';
		}
		else{
			*(ascii+2*i) = num1 + 'A' - 10;
		}
		if(num2 < 10){
			*(ascii+2*i+1) = num2 + '0';
		}
		else{
			*(ascii+2*i+1) = num2 + 'A' - 10;
		}
	}
}

#endif

