/***************************************************************************
*
*  	Filename: error.c
*	Description: Print error codes for each function
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include "log.h"
#include "data.h"
#include "memory.h"

void return_code_error(int8_t return_code, uint8_t function_name)
{
    if(return_code != 0) 
    {
	uint8_t n = 200;
    	uint8_t str[200] = "Error Return Code";
        switch(function_name)
        {
        case 1:
            switch(return_code)
            {
            case 1: my_memmove(str,(uint8_t *)"ERROR: src pointer is NULL in my_memmove\n\0",n);
                break;
            case 2: my_memmove(str,(uint8_t *)"ERROR: dst pointer is NULL in my_memmove\n\0",n);
                break;
            case 3: my_memmove(str,(uint8_t *)"ERROR: length is less than or equal to 0 in my_memmove\n\0",n);
                break;
            case 4: my_memmove(str,(uint8_t *)"ERROR: src and dst pointers did not compare correctly in my_memmove\n\0",n);
                break;
            default: my_memmove(str,(uint8_t *)"ERROR: Something unexpected occurred in my_memmove\n\0",n);
                break;
            }
            break;
        case 2:
            switch(return_code)
            {
            case 1: my_memmove(str,(uint8_t *)"ERROR: src pointer is NULL in my_memzero\n\0",n);
                break;
            case 2: my_memmove(str,(uint8_t *)"ERROR: length is less than or equal to 0 in my_memzero\n\0",n);
                break;
            case 3: my_memmove(str,(uint8_t *)"ERROR: An pointer element did not change to zero in my_memzero\n\0",n);
                break;
            default: my_memmove(str,(uint8_t *)"ERROR: Something unexpected occurred in my_memzero\n\0",n);
                break;
            }
            break;
        case 3:
            switch(return_code)
            {
            case 1: my_memmove(str,(uint8_t *)"ERROR: src pointer is NULL in my_reverse\n\0",n);
                break;
            case 2: my_memmove(str,(uint8_t *)"ERROR: length is less than or equal to 0 in my_reverse\n\0",n);
                break;
            case 3: my_memmove(str,(uint8_t *)"ERROR: my_memmove failed in my_reverse\n\0",n);
                break;
            case 4: my_memmove(str,(uint8_t *)"ERROR: src and orig_copy_src were not reverse of each other in my_reverse\n\0",n);
                break;
            default: my_memmove(str,(uint8_t *)"ERROR: Something unexpected occurred in my_reverse\n\0",n);
                break;
            }        
            break;
        case 4:
            switch(return_code)
            {
            case 1: my_memmove(str,(uint8_t *)"ERROR: src pointer is NULL in my_itoa\n\0",n);
                break;
            case 2: my_memmove(str,(uint8_t *)"ERROR: base is needs to be between 2 and 16 inclusive in my_itoa\n\0",n);
                break;
            case 3: my_memmove(str,(uint8_t *)"ERROR: error in my_reverse call in my_itoa\n\0",n);
                break;
            default: my_memmove(str,(uint8_t *)"ERROR: Something unexpected occurred in my_itoa\n\0",n);
                break;
            }   
            break;
        case 5:
            switch(return_code)
            {
            case 1: my_memmove(str,(uint8_t *)"ERROR: src pointer is NULL in my_ftoa\n\0",n);
                break;
            case 2: my_memmove(str,(uint8_t *)"ERROR: Negative float is too small to compute for this ftoa function\n\n\0",n);
                break;
            case 3: my_memmove(str,(uint8_t *)"ERROR: Float is too large to compute for this ftoa function\n\0",n);
                break;
            default: my_memmove(str,(uint8_t *)"ERROR: Something unexpected occurred in my_ftoa\n\0",n);
                break;
            }   
            break;
        case 6:
            switch(return_code)
            {
            case 1: my_memmove(str,(uint8_t *)"ERROR: str pointer is NULL in LOG_1\n\0",n);
                break;
            default: my_memmove(str,(uint8_t *)"ERROR: Something unexpected occurred in LOG_1\n\0",n);
                break;
            }   
            break;
        case 7:
            switch(return_code)
            {
            case 1: my_memmove(str,(uint8_t *)"ERROR: str pointer is NULL in LOG_2\n\0",n);
                break;
            default: my_memmove(str,(uint8_t *)"ERROR: Something unexpected occurred in LOG_2\n\0",n);
                break;
            }   
            break;
        default:
            // Something went wrong if it got in here
            my_memmove(str,(uint8_t *)"ERROR: Something unexpected occurred in return_code_error function\n\0",n);
            break;
        }

        LOG_0(str,count2null(str));
    }
}
    
