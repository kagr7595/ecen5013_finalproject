/***************************************************************************
*
*  	Filename: data.c
*	Description: Basic data manipulation
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _DATA_C
#define _DATA_C

#include <stdint.h>
#include <stdio.h>
#include "memory.h"
#include "error.h"
#include "log.h"
#include "data.h"

// Performs division on the uint128 variable type and returns the remainder
uint32_t uint128_div_remain(uint128_t * dividend, uint32_t divisor)
{
    // starts by dividing upper 32 bits
    uint64_t temp = 0;
    temp = dividend->upper%divisor;
    dividend->upper /= divisor;
    temp = dividend->upper_mid + (temp << 32);
    
    // remainder from upper 32 is added to the upper_mid 32 bits which is then divided
    dividend->upper_mid = temp/divisor;
    temp = dividend->lower_mid + ((temp%divisor) << 32);

    // remainder from upper_mid 32 is added to the lower_mid 32 bits which is then divided
    dividend->lower_mid = temp/divisor;
    temp = dividend->lower + ((temp%divisor) << 32);
    
    // remainder from lower_mid 32 is added to the lower 32 bits which is then divided
    dividend->lower = temp/divisor;

    // return remainder
    return temp%divisor;
}

// Convert data from a standard float type into an asci string
// Always 3 decimal places are printed out if there is any decimal portion
int8_t my_ftoa(uint8_t * str, float data)
{
    Float_point myFloat;
    myFloat.reg_float = data;
    myFloat.EXP = myFloat.EXP - 127;
    uint64_t man = myFloat.MANTISSA | 0x00800000;
    uint128_t fint128;

    // The parts of the float are converted to be represented as a 128 bit integer
    // This is done by breaking the 128 bit representation into 4 32 bit integers
    // upper 32
    if( myFloat.EXP >= 96) {
        if( (myFloat.EXP - (23 + 96)) > 0 ) { 
            fint128.upper = man << ((myFloat.EXP - 23) - 96);
        } else {
            fint128.upper = man >> (-1*((myFloat.EXP - 23) - 96));
        }
    } else {
        fint128.upper = 0;
    }

    // upper_mid 32
    if( myFloat.EXP >= 64) {
        if( (myFloat.EXP - (23 + 64)) > 0 ) {
            fint128.upper_mid = man << ((myFloat.EXP - 23) - 64);
        } else {
            fint128.upper_mid = man >> (-1*((myFloat.EXP - 23) - 64));
        }
    } else {
        fint128.upper_mid = 0;
    }

    // lower_mid 32
    if( myFloat.EXP >= 32) {
        if( (myFloat.EXP - (23 + 32)) > 0 ) {
            fint128.lower_mid = man << ((myFloat.EXP - 23) - 32);
        } else {
            fint128.lower_mid = man >> (-1*((myFloat.EXP - 23) - 32));
        }
    } else {
        fint128.lower_mid = 0;
    }
 
    // lower 32
    if( myFloat.EXP >= 0) {
        if( (myFloat.EXP - 23) > 0 ) {
            fint128.lower = man << (myFloat.EXP - 23);
        } else {
            fint128.lower = man >> (-1*(myFloat.EXP - 23));
        }
    } else {
        fint128.lower = 0;
    }

    float ffract = 0;
    uint16_t ffract_int = 0;
    uint16_t num_elements = 0;
    uint8_t fsign = 0;
    uint8_t base = 10;
    int8_t return_code;
    
    // if pointer is NULL, error
    if(str == NULL) {return 1;}
    
    if(data<0)
    {
	fsign = 1;
        data = data * -1;
    }
    if (data<.0001f)
    {
	ffract = 0.000;
	// change sign to 0 as 0.000 is neither negative or positive
	fsign = 0;
    } else if ((data > LARGEST_FLOAT_HANDLED_128) || (data < -1*LARGEST_FLOAT_HANDLED_128))
    {
	if (fsign == 1) { return 2;}
	else { return 3;}
    } else {
        //For the cases where the integer is really large and struct for uint128 will be used below for ftoa conversion
        //fractional portion won't exist because the exponent is too large for the mantissa to allow for this precision
        ffract = data - (float)((uint64_t)data);
    }

    //add .0005 for rounding.  The .0001 place will be cut off as rounding to 3 decimal places 
    ffract = ffract + .0005;
    
    // Finished getting correct sign, integer, and fraction parts of float number
    // Now to change from a number into an ascii and put into pointer str
    
    ffract_int = ffract*1000;
    
    /* Switch from numbers to ascii characters */
    if (ffract_int > 0)
    {
        // add fraction portion first, LSB->MSB
        for (num_elements = 0; ffract_int != 0; num_elements++)
        {
            *(str + num_elements) = ffract_int%base + '0';
            ffract_int = ffract_int/base;
        }
        // if the ffract_int is not over 99, it will need an 0.  Same with over 9 (will need two 0s)
        while (num_elements < 3)
        {
            *(str + num_elements) = '0';
            num_elements++;
        }
        
        // add decimal sign
        *(str + num_elements) = '.';
        num_elements++;
    }  

    //add if statement case for use of fint128
    for (; ((fint128.lower != 0) || (fint128.lower_mid != 0) || (fint128.upper_mid != 0) || (fint128.upper != 0)); num_elements++)
    {
        *(str + num_elements) = uint128_div_remain(&fint128, base) + '0';
    }

    // zero pad if just a fraction
    if ( (*(str + num_elements - 1)) == '.')
    {
        *(str + num_elements) = '0';
        num_elements++;
    }

    // if data is negative, change the largest bit (sign bit) in str to 1
    if (fsign)
    {
        *(str + num_elements) = '-';  
        num_elements++;
    }
    
    //reverse array of remainders to get ascii string of data without sign
    return_code = my_reverse(str, num_elements);
    if((return_code != 0) && (return_code != 2)){return 4;}

    if(return_code == 2) 
    {
        *(str + num_elements) = '0';
        num_elements++;
    }   
 
    *(str + num_elements) = '\0';  
    num_elements++;
    
    return 0;
}



// Convert data from a standard integer type into an asci string
// Need to handle signed data
int8_t my_itoa(uint8_t * str, int32_t data, int32_t base)
{
    uint32_t temp; 
    uint16_t num_elements = 0;
    uint8_t return_code = 0;
    
    
    if(str == NULL) {return 1;}
    else if(base > 16 || base < 2) {return 2;}    
    
    // if data is a negative number
    if(data < 0)
    {
        temp = ~data+1;
    } else
    {
        temp = data;
    }
    
    //switch from numbers to ascii characters
    for (num_elements = 0; temp != 0; num_elements++)
    {
        //assign remainder into the str pointer
        if(temp%base < 10)
        {
            *(str + num_elements) = temp%base + '0';
        } else
        {
            *(str + num_elements) = temp%base + ('a'-10);
        }
        temp = temp/base;
    }
    
    // if data is negative, change the largest bit (sign bit) in str to 1
    if (data < 0)
    {
        *(str+num_elements) = '-';  
        num_elements++;
    }
    
    //reverse array of remainders to get ascii string of data without sign
    return_code = my_reverse(str, num_elements);
    if(return_code != 0){return 3;}
    
    *(str+num_elements) = '\0';  
    num_elements++;
    
    return 0;
}

// Convert data from an ascii represented string into an integer type
// Assumes the ascii is a number represented in base 10
// Need to handle signed data
int32_t my_atoi(uint8_t * str)
{    
    uint32_t out_value = 0; 
    uint16_t element_num = 0; 
    uint8_t neg = 0;
    
    //returns 0 if there is no str
    if(str == NULL) {return 0;}
    
    // if data is a negative number
    if(*str == '-')
    {
        neg = 1;
        element_num++;
    }
    
    //switch from numbers to ascii characters
    while (*(str + element_num) != '\0')
    {
        out_value *= 10;
	out_value += (*(str + element_num)) - '0';        
        element_num++;
    }
    
    // if data is negative, convert to two's comp
    if (neg)
    {
        out_value = ~(out_value - 1); 
    }
    
    return out_value;
}

// Convert data types in memory from a big endian representation to 
// little endian
uint32_t big_to_little(uint32_t data)
{
    uint32_t data_out = 0x00000000;
    data_out = ((data & 0x000000FF) << 24);
    data_out |= ((data & 0x0000FF00) << 8);
    data_out |= ((data & 0x00FF0000) >> 8);
    data_out |= ((data & 0xFF000000) >> 24);
    return data_out;
}

// Convert data types in memory from a little endian representation to 
// big endian
uint32_t little_to_big(uint32_t data)
{
    uint32_t data_out = big_to_little(data);
    return data_out;
}


// Takes pointer to memory and length of bytes to print 
// and prints the hex output
void dump_memory(uint8_t * start, uint32_t length)
{

#ifndef FRDM
    uint32_t i;
    if(start == NULL) {printf("ERROR: src pointer is NULL in dump_array\n");}
    else if (length <= 0) {printf("ERROR: length is less than or equal to 0 in dump_array\n");}
    else
    {
        printf("Dump array:\n");
        for (i = 0; i < length; i++) 
        {
            printf(" array[%02d]:0x%x%x\n",i,*(start+i)>>4,(*(start+i)&0x0f));
            if (((i+1)%4)==0) {printf("\n");}
        }
    }
#endif
}


void my_ftoa_out(uint8_t * string, float data)
{    
#ifndef FRDM
    uint8_t i;

    printf("Given data: %f\n ASCII str: ",data);
    for (i = 0; *(string+i)!='\0'; i++)
    {
        printf("%c",*(string+i));
    }                       
    printf("\n\n");
#endif
}


void my_itoa_out(uint8_t * string, int32_t data, int32_t base)
{    

    uint8_t print_string[255] = "ITOA: Given data: \0";
    uint8_t print_string_base[255] = "base: \0";
    uint8_t print_string_ascii[255] = "ASCII str: \0";
    LOG_1(print_string, count2null(print_string), data, I32);
    my_newcharacter(' ',1);  
    
    LOG_1(print_string_base, count2null(print_string_base), base, I32);
    my_newcharacter(' ',1);  
    
    LOG_0(print_string_ascii,count2null(print_string_ascii));
    LOG_0(string,count2null(string));
    my_newcharacter('\n',1);  

}

uint8_t count2null(uint8_t * string)
{
	uint8_t counter = 0;
	while(*(string+counter) != '\0') {counter++;};
	return counter;
}

#endif
