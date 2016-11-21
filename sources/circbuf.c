/***************************************************************************
*
*   Filename: circbuf.c
*   Description: Implements the functions that operate on the circular buffer struct 
*                    
*   Author: Dylan Way
*       
****************************************************************************/
#include "circbuf.h"
#include "log.h"

// Creates a buffer by allocating it on the heap.
CircBuf_t * buffer_init(uint32_t buf_size) {
    // Allocates memory on the heap for the struct, meaning pointers, size element, and num_items element
    CircBuf_t *myCircBuf = malloc(sizeof(CircBuf_t));
    // Allocates memory for the buffer itself and points the *buffer pointer to the beginning of the allocated memory
    myCircBuf->buffer = malloc(sizeof(buf_size));
    
    // Sets the head and tail to the front of the buffer
    myCircBuf->head = myCircBuf->buffer;
    myCircBuf->tail = myCircBuf->buffer;
    // Sets the size to the value passed in
    myCircBuf->size = buf_size;
    // Initializes the number of items in the array to 0
    myCircBuf->num_items = 0;
    return myCircBuf;     
} 

// Checks if the circular buffer is full. Returns FULL if full, otherwise returns 0.
int8_t buffer_full(volatile CircBuf_t *cb) {
    if( cb->num_items == cb->size ) {return FULL;}
    return 0;
}

// Checks if the circular buffer is empty. Returns EMPTY if empty, otherwise returns 0.
int8_t buffer_empty(volatile CircBuf_t *cb) {
    if( cb->num_items == 0 ) {return EMPTY;}
    return 0;
}

// Adds an item to the circular buffer if the buffer is not full. If full, returns an error.
int8_t add_buffer_item(volatile CircBuf_t *cb, uint8_t item) {
    // Checks if buffer is full, returns error if true
    if( buffer_full(cb) == FULL ) {return FULL;}
    // Adds item to the circular buffer
    if( buffer_empty(cb) == EMPTY ) {}
    else if( cb->tail == (cb->buffer + cb->size - 1) ) {
        cb->tail = cb->buffer;
    }
    else {
        cb->tail = cb->tail + 1;
    }
    *(cb->tail) = item;
    cb->num_items++;
    return 0;
}

// Removes an item from the circular buffer if the buffer is not empty. If empty, returns an error.
int8_t remove_buffer_item(volatile CircBuf_t *cb, uint8_t *item) {
    // Checks if buffer is empty, returns error if true
    if( buffer_empty(cb) == EMPTY ) {return EMPTY;}
    // Removes item from the circular buffer and writes the removed item to item pointer
    *item = *(cb->head);
    if( cb->head == (cb->buffer + cb->size - 1) ) {
        cb->head = cb->buffer;
    }
    else {
        cb->head = cb->head + 1;
    }
    //cb->num_items--;
    if( cb->num_items == 1 ) {
        if( cb->tail == (cb->buffer + cb->size - 1) ) {
            cb->tail = cb->buffer;
        }
        else {
            cb->tail = cb->tail+1;
        }
    }
    cb->num_items--;
    return 0;
}

// Prints the buffer contents
int8_t print_buffer(volatile CircBuf_t *cb) {
    uint8_t count = 0;

    uint8_t print0 [256] = "\n  Buffer: ";
    LOG_0(print0,count2null(print0));
    while( count < cb->size ) {
        if ( cb->head > cb-> tail) {
            if ( cb->num_items == 0 ) {;}
            else if ( ((cb->buffer + count) >= (cb->head)) || ((cb->buffer + count) <= (cb->tail)) )
	    {
                LOG_1(0,0,(uint64_t)*(cb->buffer + count),UI8);
		my_newcharacter(' ',1);  
	    } else
		my_newcharacter(' ',2);  
        }
        else {
            if ( cb->num_items == 0 ) {;}
            else if ( ((cb->buffer + count) >= (cb->head)) && ((cb->buffer + count) <= (cb->tail)) )
	    {
                LOG_1(0,0,(uint64_t)*(cb->buffer + count),UI8);
		my_newcharacter(' ',1);  
	    } else
		my_newcharacter(' ',2);  
        }
        count++;
    }
    my_newcharacter('\n',1);  
    return 0;
}

// Destroys the buffer by freeing it from the heap
int8_t destroy_buffer(CircBuf_t **cb_ptr) {
    // Free the buffer first
    free((*cb_ptr)->buffer);
    (*cb_ptr)->buffer = NULL;
    // Then free the struct
    free(*cb_ptr);
    *cb_ptr = NULL;
    return 0;
}
