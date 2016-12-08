# ecen5013_finalproject
UART Enable:
	The compiler flag VERBOSE must be included in enable UART to a terminal from the FRDM board. This is necessary for the TSI Tests operation.

TSI Tests:
	The compiler flag TSI_TEST must be included to perform the unit tests for the capacitve touch sensor. This runs 3 tests: slider calibration, touch positon binning to 4 locations, and correlation between binning locations and the LED colors/control. The user must continue to press the given location while sending a character back to the system during these tests.

LCD Tests:
	If the compiler flag LCD_TEST is included, the LCD is initialized and runs a short test outputting basic writes to the display.  No user input is needed to run this test.

GAME:
	If neither the TSI_TEST or LCD_TEST flags are active, the game will default to the operational game mode. The game randomly selects a color: RGB, Red, Green, or Blue. The user then has a limited amount of time to select the correct color to increment their score. If the correct color isn't pressed in time, the game will end and inform the player of their final score. They will then have the operation to restart the game. Upon initialization of the game, the user will be prompted to pick a game difficulty. EASY corresponds to approximately 3 seconds to press the requested color. HARD corresponds to approximately 1.5 seconds.


