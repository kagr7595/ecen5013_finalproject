/***************************************************************************
*
*  	Filename: profiler.c
*	Description: Runs the profiler on our functions and various standard functions
*	depending on the platform
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _PROFILER_C
#define _PROFILER_C

#include <stdint.h>
#include "timer.h"
#include "data.h"
#include "memory.h"
#include "log.h"
#include "circbuf.h"
#include <string.h>

#ifndef FRDM
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#endif

#ifndef FRDM
// Returns the difference in time in clock ticks
uint64_t return_time_dif(struct timeval start, struct timeval end) {
	uint64_t secs = end.tv_sec - start.tv_sec;
	uint64_t usecs = 0;
	if (end.tv_usec < start.tv_usec) {
		usecs = (end.tv_usec + 1000000) - start.tv_usec;
		secs--;
	}
	else {
		usecs = end.tv_usec - start.tv_usec;
	}
	return (usecs + (secs*1000000));
}
#endif

void my_memmove_profiler(uint32_t size) {
	uint8_t src[size];
	uint8_t dst[size];
#ifdef FRDM
	timer_start();
	my_memmove(src, dst, size);
	uint64_t cycles = timer_end();
	uint64_t time = count_to_time(cycles);
#else
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
	my_memmove(src, dst, size);
	gettimeofday(&end, NULL);
	uint64_t time = return_time_dif(start, end);
	uint64_t cycles = (time*1000);
#endif
	uint8_t msg1[50] = "\nmy_memmove runtime (\0";
	uint8_t msg2[10] = " bytes): \0";
	uint8_t msg3[10] = " cycles, \0";
	uint8_t msg4[10] = " us \0";
	LOG_1(msg1, count2null(msg1),size,UI64);
	LOG_1(msg2, count2null(msg2),cycles,UI64);
	LOG_1(msg3, count2null(msg3),time,UI64);
	LOG_0(msg4, count2null(msg4));
}

void memmove_profiler(uint32_t size) {
	uint8_t src[size];
	uint8_t dst[size];
#ifdef FRDM
	timer_start();
	memmove(src, dst, size);
	uint64_t cycles = timer_end();
	uint64_t time = count_to_time(cycles);
#else
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
	memmove(src, dst, size);
	gettimeofday(&end, NULL);
	uint64_t time = return_time_dif(start, end);
	uint64_t cycles = (time*1000);
#endif
	uint8_t msg1[50] = "\nmemmove runtime (\0";
	uint8_t msg2[10] = " bytes): \0";
	uint8_t msg3[10] = " cycles, \0";
	uint8_t msg4[10] = " us \0";
	LOG_1(msg1, count2null(msg1),size,UI64);
	LOG_1(msg2, count2null(msg2),cycles,UI64);
	LOG_1(msg3, count2null(msg3),time,UI64);
	LOG_0(msg4, count2null(msg4));
}

void my_memzero_profiler(uint32_t size) {
	uint8_t src[size];
#ifdef FRDM
	timer_start();
	my_memzero(src, size);
	uint64_t cycles = timer_end();
	uint64_t time = count_to_time(cycles);
#else
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
	my_memzero(src, size);
	gettimeofday(&end, NULL);
	uint64_t time = return_time_dif(start, end);
	uint64_t cycles = (time*1000);
#endif
	uint8_t msg1[50] = "\nmy_memzero runtime (\0";
	uint8_t msg2[10] = " bytes): \0";
	uint8_t msg3[10] = " cycles, \0";
	uint8_t msg4[10] = " us \0";
	LOG_1(msg1, count2null(msg1),size,UI64);
	LOG_1(msg2, count2null(msg2),cycles,UI64);
	LOG_1(msg3, count2null(msg3),time,UI64);
	LOG_0(msg4, count2null(msg4));
}

void memset_profiler(uint32_t size) {
	uint8_t src[size];
#ifdef FRDM
	timer_start();
	memset(src, 0, size);
	uint64_t cycles = timer_end();
	uint64_t time = count_to_time(cycles);
#else
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
	memset(src, 0, size);
	gettimeofday(&end, NULL);
	uint64_t time = return_time_dif(start, end);
	uint64_t cycles = (time*1000);
#endif
	uint8_t msg1[50] = "\nmemset runtime (\0";
	uint8_t msg2[10] = " bytes): \0";
	uint8_t msg3[10] = " cycles, \0";
	uint8_t msg4[10] = " us \0";
	LOG_1(msg1, count2null(msg1),size,UI64);
	LOG_1(msg2, count2null(msg2),cycles,UI64);
	LOG_1(msg3, count2null(msg3),time,UI64);
	LOG_0(msg4, count2null(msg4));
}

void my_reverse_profiler(uint32_t size) {
	uint8_t src[size];
#ifdef FRDM
	timer_start();
	my_reverse(src, size);
	uint64_t cycles = timer_end();
	uint64_t time = count_to_time(cycles);
#else
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
	my_reverse(src, size);
	gettimeofday(&end, NULL);
	uint64_t time = return_time_dif(start, end);
	uint64_t cycles = (time*1000);
#endif
	uint8_t msg1[50] = "\nmy_reverse runtime (\0";
	uint8_t msg2[10] = " bytes): \0";
	uint8_t msg3[10] = " cycles, \0";
	uint8_t msg4[10] = " us \0";
	LOG_1(msg1, count2null(msg1),size,UI64);
	LOG_1(msg2, count2null(msg2),cycles,UI64);
	LOG_1(msg3, count2null(msg3),time,UI64);
	LOG_0(msg4, count2null(msg4));
}

void my_itoa_profiler() {
	uint8_t str[10];
	uint32_t data = 123456;
#ifdef FRDM
	timer_start();
	my_itoa(str, data, 10);
	uint64_t cycles = timer_end();
	uint64_t time = count_to_time(cycles);
#else
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
	my_itoa(str, data, 10);
	gettimeofday(&end, NULL);
	uint64_t time = return_time_dif(start, end);
	uint64_t cycles = (time*1000);
#endif
	uint8_t msg1[50] = "\nmy_itoa runtime \0";
	uint8_t msg2[10] = " cycles, \0";
	uint8_t msg3[10] = " us \0";
	LOG_1(msg1, count2null(msg1),cycles,UI64);
	LOG_1(msg2, count2null(msg2),time,UI64);
	LOG_0(msg3, count2null(msg3));
}

void my_ftoa_profiler() {
	uint8_t str[40];
	float data = 123456789E20;
#ifdef FRDM
	timer_start();
	my_ftoa(str, data);
	uint64_t cycles = timer_end();
	uint64_t time = count_to_time(cycles);
#else
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
	my_ftoa(str, data);
	gettimeofday(&end, NULL);
	uint64_t time = return_time_dif(start, end);
	uint64_t cycles = (time*1000);
#endif
	uint8_t msg1[50] = "\nmy_ftoa runtime \0";
	uint8_t msg2[10] = " cycles, \0";
	uint8_t msg3[10] = " us \0";
	LOG_1(msg1, count2null(msg1),cycles,UI64);
	LOG_1(msg2, count2null(msg2),time,UI64);
	LOG_0(msg3, count2null(msg3));
}

void my_atoi_profiler() {
	uint8_t str[6] = "123456";
#ifdef FRDM
	timer_start();
	my_atoi(str);
	uint64_t cycles = timer_end();
	uint64_t time = count_to_time(cycles);
#else
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
	my_atoi(str);
	gettimeofday(&end, NULL);
	uint64_t time = return_time_dif(start, end);
	uint64_t cycles = (time*1000);
#endif
	uint8_t msg1[50] = "\nmy_atoi runtime \0";
	uint8_t msg2[10] = " cycles, \0";
	uint8_t msg3[10] = " us \0";
	LOG_1(msg1, count2null(msg1),cycles,UI64);
	LOG_1(msg2, count2null(msg2),time,UI64);
	LOG_0(msg3, count2null(msg3));
}

#ifndef FRDM
void atoi_profiler() {
	const char str[6] = "123456";
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
	int result = atoi(str);
	gettimeofday(&end, NULL);
	uint64_t time = return_time_dif(start, end);
	uint64_t cycles = (time*1000);
	uint8_t msg1[50] = "\natoi runtime (\0";
	uint8_t msg2[10] = " cycles, \0";
	uint8_t msg3[10] = " us \0";
	LOG_1(msg1, count2null(msg1),cycles,UI64);
	LOG_1(msg2, count2null(msg2),time,UI64);
	LOG_0(msg3, count2null(msg3));
}
#endif

void malloc_empty_profiler(uint32_t size) {
	uint8_t *ptr;
#ifdef FRDM
	timer_start();
        ptr = malloc(size);
	uint64_t cycles = timer_end();
	uint64_t time = count_to_time(cycles);
#else
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
        ptr = malloc(size);
	gettimeofday(&end, NULL);
	uint64_t time = return_time_dif(start, end);
	uint64_t cycles = (time*1000);
#endif
	free(ptr);
	uint8_t msg1[50] = "\nmalloc (empty) runtime (\0";
	uint8_t msg2[10] = " bytes): \0";
	uint8_t msg3[10] = " cycles, \0";
	uint8_t msg4[10] = " us \0";
	LOG_1(msg1, count2null(msg1),size,UI64);
	LOG_1(msg2, count2null(msg2),cycles,UI64);
	LOG_1(msg3, count2null(msg3),time,UI64);
	LOG_0(msg4, count2null(msg4));
}

void malloc_nonempty_profiler(uint32_t size) {
	uint8_t *ptr;
	uint8_t *ptr2;
	ptr = malloc(100);
#ifdef FRDM
	timer_start();
        ptr2 = malloc(size);
	uint64_t cycles = timer_end();
	uint64_t time = count_to_time(cycles);
#else
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
        ptr2 = malloc(size);
	gettimeofday(&end, NULL);
	uint64_t time = return_time_dif(start, end);
	uint64_t cycles = (time*1000);
#endif
	free(ptr);
	free(ptr2);
	uint8_t msg1[50] = "\nmalloc (non-empty) runtime (\0";
	uint8_t msg2[10] = " bytes): \0";
	uint8_t msg3[10] = " cycles, \0";
	uint8_t msg4[10] = " us \0";
	LOG_1(msg1, count2null(msg1),size,UI64);
	LOG_1(msg2, count2null(msg2),cycles,UI64);
	LOG_1(msg3, count2null(msg3),time,UI64);
	LOG_0(msg4, count2null(msg4));
}

void free_profiler(uint32_t size) {
	uint8_t *ptr;
        ptr = malloc(size);
#ifdef FRDM
	timer_start();
	free(ptr);
	uint64_t cycles = timer_end();
	uint64_t time = count_to_time(cycles);
#else
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
	free(ptr);
	gettimeofday(&end, NULL);
	uint64_t time = return_time_dif(start, end);
	uint64_t cycles = (time*1000);
#endif
	uint8_t msg1[50] = "\nfree runtime (\0";
	uint8_t msg2[10] = " bytes): \0";
	uint8_t msg3[10] = " cycles, \0";
	uint8_t msg4[10] = " us \0";
	LOG_1(msg1, count2null(msg1),size,UI64);
	LOG_1(msg2, count2null(msg2),cycles,UI64);
	LOG_1(msg3, count2null(msg3),time,UI64);
	LOG_0(msg4, count2null(msg4));
}

void circbuf_add_profiler() {
	CircBuf_t *myCB = buffer_init(5);
#ifdef FRDM
	timer_start();
	add_buffer_item(myCB, '1');
	uint64_t cycles = timer_end();
	destroy_buffer(&myCB);
	uint64_t time = count_to_time(cycles);
#else
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
	add_buffer_item(myCB, '1');
	gettimeofday(&end, NULL);
	uint64_t time = return_time_dif(start, end);
	uint64_t cycles = (time*1000);
	destroy_buffer(&myCB);
#endif
	uint8_t msg1[50] = "\nadd circular buffer runtime: \0";
	uint8_t msg2[10] = " cycles, \0";
	uint8_t msg3[10] = " us \0";
	LOG_1(msg1, count2null(msg1),cycles,UI64);
	LOG_1(msg2, count2null(msg2),time,UI64);
	LOG_0(msg3, count2null(msg3));
}

void circbuf_remove_profiler() {
	CircBuf_t *myCB = buffer_init(5);
	uint8_t item;
	uint8_t *item_ptr = &item;
	add_buffer_item(myCB, '1');
#ifdef FRDM
	timer_start();
	remove_buffer_item(myCB, item_ptr);
	uint64_t cycles = timer_end();
	destroy_buffer(&myCB);
	uint64_t time = count_to_time(cycles);
#else
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
	remove_buffer_item(myCB, item_ptr);
	gettimeofday(&end, NULL);
	uint64_t time = return_time_dif(start, end);
	uint64_t cycles = (time*1000);
	destroy_buffer(&myCB);
#endif
	uint8_t msg1[50] = "\nremove circular buffer runtime: \0";
	uint8_t msg2[10] = " cycles, \0";
	uint8_t msg3[10] = " us \0";
	LOG_1(msg1, count2null(msg1),cycles,UI64);
	LOG_1(msg2, count2null(msg2),time,UI64);
	LOG_0(msg3, count2null(msg3));
}

void log_profiler() {
	uint8_t print[35] = "\n   Test message for profiler. \0";
	timer_start();
	LOG_0(print, count2null(print));
	uint64_t cycles = timer_end();
	uint64_t time = count_to_time(cycles);
	uint8_t msg1[50] = "\nLOG runtime: \0";
	uint8_t msg2[10] = " cycles, \0";
	uint8_t msg3[10] = " us \0";
	LOG_1(msg1, count2null(msg1),cycles,UI64);
	LOG_1(msg2, count2null(msg2),time,UI64);
	LOG_0(msg3, count2null(msg3));
}

#ifndef FRDM
void print_profiler(uint8_t option) {	
	// runs profiler on print statements	
	int32_t val1 = 1234;
	int32_t val2 = 5678;
	int32_t val3 = 1246;
	float fval1 = 123456;
	float fval2 = 789012;
	float fval3 = 147852;
	struct timeval start;
	struct timeval end;
	gettimeofday(&start, NULL);
	if (option == 0) {
		printf("\n20 character string!");
	}
	else if (option == 1) {
		printf("\n%d", val1);
	}
	else if (option == 2) {
		printf("\n%d %d", val1, val2);
	}
	else if (option == 3) {
		printf("\n%d %d %d", val1, val2, val3);
	} 
	else if (option == 4) {
		printf("\n%f", fval1);
	} 
	else if (option == 5) {
		printf("\n%f %f", fval1, fval2);
	} 
	else {
		printf("\n%f %f %f", fval1, fval2, fval3);
	} 
	gettimeofday(&end, NULL);
	uint64_t time = return_time_dif(start, end);
	uint64_t cycles = (time*1000);
	uint8_t msg1[50] = "\nprintf runtime: \0";
	uint8_t msg2[10] = " cycles, \0";
	uint8_t msg3[10] = " us \0";
	LOG_1(msg1, count2null(msg1),cycles,UI64);
	LOG_1(msg2, count2null(msg2),time,UI64);
	LOG_0(msg3, count2null(msg3));
}
#endif

void profiler_tests() {
#ifdef FRDM
	init_timer();
#endif
	my_memmove_profiler(10);
	my_memmove_profiler(100);
	my_memmove_profiler(1000);
	my_memmove_profiler(5000);
	
	memmove_profiler(10);
	memmove_profiler(100);
	memmove_profiler(1000);
	memmove_profiler(5000);

	my_memzero_profiler(10);
	my_memzero_profiler(100);
	my_memzero_profiler(1000);
	my_memzero_profiler(5000);
	
	memset_profiler(10);
	memset_profiler(100);
	memset_profiler(1000);
	memset_profiler(5000);

	my_reverse_profiler(10);
	my_reverse_profiler(100);
	my_reverse_profiler(1000);
	my_reverse_profiler(5000);

	my_itoa_profiler();
	my_ftoa_profiler();
	my_atoi_profiler();

#ifndef FRDM
	atoi_profiler();
#endif

	malloc_empty_profiler(10);
	malloc_empty_profiler(100);
	malloc_empty_profiler(500);
	malloc_empty_profiler(1000);

	malloc_nonempty_profiler(10);
	malloc_nonempty_profiler(100);
	malloc_nonempty_profiler(500);
	malloc_nonempty_profiler(1000);

	free_profiler(10);
	free_profiler(100);
	free_profiler(500);
	free_profiler(1000);

	circbuf_add_profiler();
	circbuf_remove_profiler();

#ifdef FRDM
	log_profiler();
#else
	print_profiler(0);
	print_profiler(1);
	print_profiler(2);
	print_profiler(3);
	print_profiler(4);
	print_profiler(5);
	print_profiler(6);
#endif
}

#endif
