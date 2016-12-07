/***************************************************************************
*
*  	Filename: gameSS.c
*	Description: Contains functions that store the current game state and transition
*	between different states
*                    
*       Author: Kathy Grimes 
*               Dylan Way
*       
****************************************************************************/
#ifndef _GAMESS_C
#define _GAMESS_C

#include "gameSS.h"

uint8_t state = DIFFICULTY;
uint8_t return_state;
uint64_t score = 0;
extern volatile uint8_t TIMER_EVENT;
extern volatile uint8_t COLOR_EVENT;
extern volatile uint8_t RELEASE_EVENT;
extern volatile uint8_t touch_color;
uint8_t speed;
uint8_t req_color;
uint8_t difficulty;

// This infinite loop keeps track of the state machine
uint8_t game_run(){
	NVIC_DisableIRQ(TSI0_IRQn);
	TIMER_EVENT = 0;
	COLOR_EVENT = 0;
	RELEASE_EVENT = 0;
	NVIC_EnableIRQ(TSI0_IRQn);

	uint8_t error = NO_GAME_ERROR;

	while(1){
		if(state == DIFFICULTY){
			game_difficulty();
		}
		else if(state == START){
			game_start();
		}
		else if(state == WAIT){
			game_wait();
		}
		else if(state == NEW_COLOR){
			game_request_color();
		}
		else if(state == WAIT_RELEASE){
			game_wait_release();
		}
		else if(state == END){
			game_end();
		}

		if(error) {
			uint8_t str_err[20] = "There was an error!\n";
			LOG_0(str_err,20);
			while(1){;}
		}
	}
}

// Sets the game difficulty
uint8_t game_difficulty(){
	uint8_t error = NO_GAME_ERROR;
	RELEASE_EVENT = 0;
	COLOR_EVENT = 0;
	uint8_t str_dif[48] = "Select a difficulty!\nLeft = EASY   Right = HARD\n";
	uint8_t str_dif_lcd[32] = "Touch Left=EASY Touch Right=HARD";
	lcd_data_write(str_dif_lcd,32);
	LOG_0(str_dif, 48);
	lcd_delay(MEDIUM);
	while(!COLOR_EVENT){
		NVIC_DisableIRQ(TSI0_IRQn);
		RELEASE_EVENT = 0;
		NVIC_EnableIRQ(TSI0_IRQn);
	}
	if((touch_color == RGB) || (touch_color == RED)){
		uint8_t str_dif1[15] = "Selected EASY!\n";
		lcd_data_write(str_dif1,14);
		LOG_0(str_dif1, 15);
		difficulty = EASY;
		lcd_delay(MEDIUM);
	}
	else if((touch_color == GREEN) || (touch_color == BLUE)){
		uint8_t str_dif2[15] = "Selected HARD!\n";
		lcd_data_write(str_dif2,14);
		LOG_0(str_dif2, 15);
		difficulty = HARD;
		lcd_delay(MEDIUM);
	}
	else{
		error = DIF_GAME_ERROR;
	}
	uint8_t str[37] = "To start the game, touch the slider!\n";
	uint8_t str_lcd[32] = " To start game,   touch slider!";
	lcd_data_write(str_lcd,31);
	LOG_0(str, 37);
	state = WAIT_RELEASE;
	return_state = START;
	return error;
}

// Initializes the game
uint8_t game_start(){
	uint8_t error = NO_GAME_ERROR;
	if(COLOR_EVENT){
		if(difficulty == EASY){
			timer_start(SLOW);
		}
		else if(difficulty == HARD){
			timer_start(FAST);
		}
		else{
			error = START_GAME_ERROR;
		}
		state = WAIT_RELEASE;
		return_state = NEW_COLOR;
	}
	return error;
}

// Wait state stalls until a new color is selected (right or wrong) or the timer expires
uint8_t game_wait(){
	uint8_t error = NO_GAME_ERROR;
	if(TIMER_EVENT){
		state = END;
	}
	else if(COLOR_EVENT){
		if(touch_color == req_color){
			score++;
			state = NEW_COLOR;
		}
		else{
			state = WAIT_RELEASE;
			return_state = WAIT;
		}
	}
	else{
		NVIC_DisableIRQ(TSI0_IRQn);
		RELEASE_EVENT = 0;
		NVIC_EnableIRQ(TSI0_IRQn);
	}
	return error;
}

// Requests the next color from the player following a correct move or the start of the game
uint8_t game_request_color(){
	uint8_t error = NO_GAME_ERROR;
	// temporary log function to simulate LCD usage
	int8_t color_num = rand() % 4;
	//int8_t color_num = 3;
	if(color_num == 0){
		req_color = RED;
		uint8_t str[4] = "RED\n";
		uint8_t str_lcd[16] = "      RED     ";
		LOG_0(str, 4);
		lcd_data_write(str_lcd,13);
	}
	else if(color_num == 1){
		req_color = GREEN;
		uint8_t str[6] = "GREEN\n";
		uint8_t str_lcd[16] = "     GREEN    ";
		LOG_0(str, 6);
		lcd_data_write(str_lcd,13);
	}
	else if(color_num == 2){
		req_color = BLUE;
		uint8_t str[5] = "BLUE\n";
		uint8_t str_lcd[16] = "      BLUE    ";
		LOG_0(str, 5);
		lcd_data_write(str_lcd,13);
	}
	else if(color_num == 3){
		req_color = RGB;
		uint8_t str[4] = "RGB\n";
		uint8_t str_lcd[16] = "      RGB     ";
		LOG_0(str, 4);
		lcd_data_write(str_lcd,13);
	}
	else{
		error = COLOR_REQUEST_GAME_ERROR;
	}

	if(difficulty == EASY){
		timer_start(SLOW);
	}
	else if(difficulty == HARD){
		timer_start(FAST);
	}
	else{
		error = COLOR_REQUEST_GAME_ERROR;
	}
	state = WAIT_RELEASE;
	return_state = WAIT;
	return error;
}

// Wait for the button to be released
uint8_t game_wait_release(){
	uint8_t error = NO_GAME_ERROR;
	if(TIMER_EVENT){
		state = END;
	}
	else if(RELEASE_EVENT){
		NVIC_DisableIRQ(TSI0_IRQn);
		COLOR_EVENT = 0;
		RELEASE_EVENT = 0;
		NVIC_EnableIRQ(TSI0_IRQn);

		state = return_state;
	}
	return error;
}

// Game goes into this state when a player hasn't touched the correct color fast enough
uint8_t game_end(){
	uint8_t error = NO_GAME_ERROR;
	uint8_t value[15] = "0 ";
	timer_end();
	NVIC_DisableIRQ(TSI0_IRQn);
	TIMER_EVENT = 0;
	NVIC_EnableIRQ(TSI0_IRQn);
	uint8_t str[19] = "\nGAME OVER\n SCORE: ";
	uint8_t str1[35] = "\n\nTo play again, touch the slider!\n";
	uint8_t str_lcd[26]  = "   GAME OVER      SCORE = ";
	uint8_t str_lcd1[32] = " To play again,   touch slider! ";
	my_itoa(value,score,10);
	lcd_data_write(str_lcd,26);
	lcd_data_write_append(value,count2null(value));
	lcd_delay(MEDIUM);
	LOG_1(str, 19, score, UI64);
	lcd_data_write(str_lcd1,31);
	LOG_0(str1, 35);
	score = 0;
	state = WAIT_RELEASE;
	return_state = START;
	return error;
}

#endif
