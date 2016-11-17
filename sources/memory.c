/***************************************************************************
*
*  	Filename: memory.c
*	Description: Basic memory movement options
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _MEMORY_C
#define _MEMORY_C

#include <stdint.h>
#include <stdio.h>
#include "memory.h"
#ifdef FRDM
#include "dma.h"
#endif

// Take two pointers (one source and one destination) and a length of bytes
// to move from one location to the other
// Checks overlapped areas between the two memory blocks
// Returns error if move failed
int8_t my_memmove_byte(uint8_t * src, uint8_t * dst, uint32_t length)
{
    uint32_t i;
    
    // if src pointer is NULL
    if (src == NULL) {return 1;}
    // if dst pointer is NULL
    else if (dst == NULL) {return 2;}
    // if length is less than or equal to 0
    else if (length <= 0 ) {return 3;}
    // if the memory locations for both both arrays aren't distinct, return 5
    else if (src == dst) {return 0;}//do nothing as the data is the same and not worth it to waste clock cycles
    //src addresses overlap with dst (where src occurs first before dst)
    else if ((src < dst) && (src + length-1 > dst))
    {
#ifdef MY_DMA
    	dma_transfer(src+(dst-src),dst+(dst-src),length-(uint32_t)(dst-src),1);
    	dma_transfer(src,dst,(uint32_t)(dst-src),1);
#else
    	// move bytes from src array to dst array
		for (i = 0; i < length; i++)
		{
			*(dst+(length-1)-i) = *(src+(length-1)-i);
		}
#endif
	}
    // if no condition matches take pointer and length and reverse byte order
    else 
    {
#ifdef MY_DMA
    	dma_transfer(src,dst,length,1);
#else
    	// move bytes from src array to dst array
        for (i = 0; i < length; i++)
        {
            *(dst+i) = *(src + i);
        }
#endif
    }
    
    return 0;
}

// Take two pointers (one source and one destination) and a length of bytes (more than 4)
// to move from one location to the other
// Checks overlapped areas between the two memory blocks
// Returns error if move failed
// Only works when both src and dst are aligned - if not aligned will call my_memmove_byte until both aligned
int8_t my_memmove(uint8_t * src, uint8_t * dst, uint32_t length)
{
	uint32_t i = 0;
	uint8_t rem_src = (4-(uint32_t)src%4)%4;
	uint8_t rem_dst = (4-(uint32_t)dst%4)%4;
	uint32_t * aligned_src;
	uint32_t * aligned_dst;

    // if src pointer is NULL
    if (src == NULL) {return 1;}
    // if dst pointer is NULL
    else if (dst == NULL) {return 2;}
    // if length is less than or equal to 0
    else if (length <= 0 ) {return 3;}
    // if the memory locations for both both arrays aren't distinct, return 5
    else if (src == dst) {return 0;}//do nothing as the data is the same and not worth it to waste clock cycles
    //src addresses overlap with dst (where src occurs first before dst) and is at least 32 bytes address difference between src and dst
#ifndef MY_DMA
    else if (length > 3)
    {
    	if ((src+3<dst) && (src+(length-1) > dst))
		{
			if(rem_src==rem_dst)
			{
				uint8_t rem_end_src = ((uint32_t)src + length)%4;
				uint8_t * src_end = src + length;
				uint8_t * dst_end = dst + length;

				//start from end of array and find out if it is aligned - if not, then do byte mode until alignment occurs
				if(rem_end_src>0)
					my_memmove_byte(src_end-(rem_end_src-1), dst_end-(rem_end_src-1),rem_end_src);

				aligned_src = (uint32_t *)(src_end-rem_end_src);
				aligned_dst = (uint32_t *)(dst_end-rem_end_src);
				length = length - rem_end_src;

				for(i = 1; i< length/4 + 1; i++)
				{
					*(aligned_dst - i) = *(aligned_src - i);
				}

				length = length - (i-1)*4;
				if(length>0)
					my_memmove_byte(src,dst,length);
			}else
			{
				//byte mode(call my_memmove_byte)
				my_memmove_byte(src,dst,length);
			}
		} else
		{
			if(rem_src==rem_dst)
			{
				//do a 32 bit mode, first find out if src%4 is on a boundary, if not then do byte mode to start
				if(rem_src!=0)
				{
					my_memmove_byte(src,dst,rem_src);
				}
				length = length - rem_src;
				aligned_src = (uint32_t *)src + rem_src;
				aligned_dst = (uint32_t *)dst + rem_src;
				rem_src = (uint32_t)src%4;

				for(i = 0; i < length/4; i++)
				{
					*(aligned_dst + i) = *(aligned_src + i);
				}
				length = length - i*4;
				if(length > 0)
					my_memmove_byte(src+i*4,dst+i*4,length);
			}else
			{
				//byte mode(call my_memmove_byte)
				my_memmove_byte(src,dst,length);
			}
		}
	}
#endif
    else {
		my_memmove_byte(src,dst,length);
	}
	return 0;
}



// Takes pointer to memory location and a length in bytes, 
// then zeros out all the memroy
// Returns error if the zeroing out failed
int8_t my_memzero(uint8_t * src, uint32_t length)
{
    uint32_t i;
    
    // if arraay pointer is NULL
    if (src == NULL) {return 1;}
    // if length is less than or equal to 0
    else if (length <= 0 ) {return 2;}
    // if no condition matches take pointer and length and reverse byte order
    else 
    {
#ifdef MY_DMA
    	uint8_t zero [4] = "0000";
    	uint8_t align = 4-(uint32_t)src%4;
    	if((uint32_t)src%4!=0)
    	{
    		dma_transfer(zero,src,align,0);
    		length = length-align;
    		src = src + align;
    	}
    	dma_transfer(zero,src,length,0);
#else
    	//if there are transfers that can do 32 bits at once, do them first
    	for(i = 0; i< length/4; i++)
    	{
    		*(src + 0 + i*4) = '0';
    		*(src + 1 + i*4) = '0';
    		*(src + 2 + i*4) = '0';
    		*(src + 3 + i*4) = '0';
    	}
    	src = src+i*4;
        for (i = 0; i < length%4; i++)
        {
            *(src + i) = '0';
        }
#endif
    }
    return 0;
}


// Takes pointer to memory location and a length in bytes,
// then reverse order of all bytes
// Returns error if reverse failed
int8_t my_reverse(uint8_t * src, uint32_t length)
{
    uint32_t i;
    uint8_t temp;
    uint8_t return_code = 0;
    
    // if arraay pointer is NULL
    if (src == NULL) {return 1;}
    // if length is less than or equal to 0
    else if (length <= 0 ) {return 2;}
    // if no condition matches take pointer and length and reverse byte order
    else 
    {
        uint8_t orig_copy_src[length];
        // make a copy of the original src before modification
        // use this copy to compare the reverse function to the original data
        return_code = my_memmove(src, orig_copy_src, length);
        if (return_code != 0 ) {return 3;} 
        
        // reverse all the bytes in the src pointer
        for (i = 0; i < length/2; i++)
        {
            temp = *(src+i);
            *(src+i) = *(src+(length-1-i));
            *(src+length-1-i) = temp;
        }
        
        // compare orig_copy_src (highest to lowest byte) to 
        // reversed src (lowest to highest byte)
        // output error if different
        for (i = 0; i < length; i++)
        {
            if (*(orig_copy_src+(length-1-i)) != *(src+i)) {return 4;}
        }
    }
    return 0;
}

#endif
