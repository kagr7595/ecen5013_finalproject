/***************************************************************************
*
*  	Filename: profiler.h
*	Description: Header file for profiler.c
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _PROFILER_H
#define _PROFILER_H

void my_memmove_profiler(uint32_t size);

void memmove_profiler(uint32_t size);

void my_memzero_profiler(uint32_t size);

void memset_profiler(uint32_t size);

void my_reverse_profiler(uint32_t size);

void my_itoa_profiler(void);

void my_ftoa_profiler(void);

void my_atoi_profiler(void);

void malloc_empty_profiler(uint32_t size);

void malloc_nonempty_profiler(uint32_t size);

void free_profiler(uint32_t size);

void circbuf_add_profiler(void);

void circbuf_remove_profiler(void);

void log_profiler(void);

void my_memmove_profiler(uint32_t size);

void profiler_tests(void);

#endif
