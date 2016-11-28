/***************************************************************************
*
*  	Filename: gameSS.h
*	Description: Header file for gameSS.c
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _GAMESS_H
#define _GAMESS_H

/* Defines and Structures section
 ***************************************************************************/
#include "RGB.h"
#include "LED_Control.h"
#include "tsi.h"
#include "LOG.h"
#include "timer.h"
#include <stdint.h>
#include <stdlib.h>
#include "core_cm0plus.h"
#include "lcd.h"

// Defines the game states
enum GAME_STATES {DIFFICULTY, START, WAIT, NEW_COLOR, WAIT_RELEASE, END};
// Defines the game difficulties
enum DIFF {EASY, HARD};
// Defines the potential errors
typedef enum Game_Errors_t {
	NO_GAME_ERROR = 0x00,
	DIF_GAME_ERROR = 0x01,
	START_GAME_ERROR = 0x02,
	COLOR_REQUEST_GAME_ERROR = 0x03,
} Cmd_Errors;

/* Function prototype Section
 * Add prototypes for all functions called by this module, with the exception
 * of runtime routines.
 ***************************************************************************/

// This infinite loop keeps track of the state machine
uint8_t game_run();

// Sets the game difficulty
uint8_t game_difficulty();

// Initializes the game
uint8_t game_start();

// Wait state stalls until a new color is selected (right or wrong) or the timer expires
uint8_t game_wait();

// Requests the next color from the player following a correct move or the start of the game
uint8_t game_request_color();

// Wait for the button to be released
uint8_t game_wait_release();

// Game goes into this state when a player hasn't touched the correct color fast enough
uint8_t game_end();

#endif
