/***************************************************************************
*
*  	Filename: data.h
*	Description: Header file for data.c
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _DATA_H
#define _DATA_H

#include <stdint.h>
#include <stdio.h>
#include "memory.h"
#include "error.h"

#ifdef FRDM
#pragma GCC poison printf
#endif

/* Defines and Structures section
 ***************************************************************************/
#define LARGEST_FLOAT_HANDLED_64 18446744073709551615.999999
#define LARGEST_FLOAT_HANDLED_128 3.4028236692093846349337E38

// Floating point structure
typedef union {
    struct {	
        uint32_t MANTISSA : 23;
        int32_t EXP : 8;
        uint32_t SIGN : 1;
    };
    float reg_float;
    uint32_t reg_uint32;
} Float_point;

typedef struct uint128 {
    uint32_t upper;
    uint32_t upper_mid;
    uint32_t lower_mid;
    uint32_t lower;
} uint128_t;

/* Function prototype Section
 * Add protoypes for all functions called by this module, with the exception
 * of runtime routines.
 ***************************************************************************/
// Performs division on the uint128 variable type and returns the remainder
uint32_t uint128_div_remain(uint128_t * dividend, uint32_t divisor);

// Convert data from a standard integer type into an asci string
// Need to handle signed data
int8_t my_itoa(uint8_t * str, int32_t data, int32_t base);

// Convert data from an ascii represented string into an integer type
// Need to handle signed data
int32_t my_atoi(uint8_t * str);

// Convert data types in memory from a big endian representation to 
// little endian
uint32_t big_to_little(uint32_t data);

// Convert data types in memory from a little endian representation to 
// big endian
uint32_t little_to_big(uint32_t data);

// Takes pointer to memory and length of bytes to print 
// and prints the hex output
void dump_memory(uint8_t * start, uint32_t length);

// Prints output for my_itoa function
void my_itoa_out(uint8_t * string, int32_t data, int32_t base);

// Convert data from a standard float type into an asci string
int8_t my_ftoa(uint8_t * str, float data);

// Prints output for my_ftoa function
void my_ftoa_out(uint8_t * string, float data);

// Count length of asci string until '/0'
uint8_t count2null(uint8_t * string);


#endif
