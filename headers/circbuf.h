/***************************************************************************
*
*   Filename: circbuf.h
*   Description: Header file for circbuf.c
*                    
*   Author: Dylan Way
*       
****************************************************************************/
#ifndef _CIRCBUF_H
#define _CIRCBUF_H

/* Included libraries */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/* Defines section */
// Circular buffer definition from class lecture
typedef struct CircBuf {
    uint8_t * buffer;    // Beginning of buffer in memory
    uint8_t * head;      // Pointer to oldest
    uint8_t * tail;      // Pointer to newest
    uint8_t   size;      // Size of the buffer (number of total items)
    uint32_t  num_items; // Current number of items
} CircBuf_t;

// Enum for the circular buffer states
enum buffer_states 
{
    PARTFILL,
    EMPTY,
    FULL
} buf_state;

/* Function prototype Section */

// Initializes a circular buffer of the requested size and returns a pointer to that buffer.
CircBuf_t * buffer_init(uint32_t buf_size);

// Checks if the circular buffer is full. Returns 1 if full, otherwise returns 0.
int8_t buffer_full(volatile CircBuf_t *cb);

// Checks if the circular buffer is empty. Returns 1 if empty, otherwise returns 0.
int8_t buffer_empty(volatile CircBuf_t *cb);

// Adds an item to the circular buffer if the buffer is not full. If full, returns an error.
int8_t add_buffer_item(volatile CircBuf_t *cb, uint8_t item);

// Removes an item from the circular buffer if the buffer is not empty. If empty, returns an error.
int8_t remove_buffer_item(volatile CircBuf_t *cb, uint8_t *item);

// Prints buffer contents
int8_t print_buffer(volatile CircBuf_t *cb);

// Destroys the circular buffer freeing it from dynamic memory
int8_t destroy_buffer(CircBuf_t **cb_ptr);

#endif
