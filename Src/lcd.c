/**
 * Name: Lukas Schmidt
 * Course: CPE2610
 * Section: 111
 * Assignment: Lab 4 LCD Display
 * File: lcd.c
 * Dependencies:
 * 		gpio.h - gpio structs
 * Usage:
 */

//Includes
//Necessary standard libraries
#include <lcd.h>
#include <stdbool.h>

//Custom header files

//File static pointers to necessary gpio ports
static volatile GPIO_t* const gpioa = (volatile GPIO_t*) GPIOA_BASE;
static volatile GPIO_t* const gpioc = (volatile GPIO_t*) GPIOC_BASE;

//Static helper functions
/**
 * lcdWriteCmd()
 * Description:
 * 		Sends command over to display.
 * 		Steps:
 * 		1. E=1,RS->CMD,RW->W
 * 		2. CMD-> DB
 * 		3. E=0,RS->CMD,RW->W
 * 		4. Wait appropriate delay or (EC)Poll BF
 * 	Inputs:
 * 		cmd(uint8_t) - cmd to execute on the display
 * 	Outputs: None
 */
static void lcdWriteCmd(uint8_t cmd) {
	//DB is PA[11..4]
	//PC8 is RS
	//PC9 is RW
	
	//set RS to command
	gpioc->bsrr = RS_CMD;
	//set RW to write
	gpioc->bsrr = RW_WR;
	//set E to 1
	gpioc->bsrr = E_SET;
	//write cmd to databus (read modify write)
	uint32_t reg = gpioa->odr;
	reg &= 0xFFFFF00F;
	reg |= (cmd<<4);
	gpioa->odr = reg;
	//set E to 0
	gpioc->bsrr = E_CLR;
	//wait for delay
	BusyMilli(60);

}


/** EXTRA CREDIT
 * lcdWaitBusyFlag()
 * Description:
 * 		Blocks polling the BusyFlag until it reads '0'
 * 		Note: The mode of DB7 must be switched to input during polling
 * 				and back to output after polling is complete.
 */
static void lcdWaitBusyFlag();





/**
 * LcdSetup()
 * Description:
 * 		Configure PA[11..4] for use as 8-bit output Databus(DB)
 * 		Configure PC8 as output for RS
 * 		Configure PC9 as output for RW
 * 		Configure PC10 as output for R10
 * 		**For EC: Turn of Input Resistors in PA[11..4]
 * Inputs: None
 * Outputs: None
 */
void LcdSetup(){
	//allow the clock to be enabled
	    volatile uint32_t* rcc = (uint32_t*) RCC_AHB1_ENR;
	    *rcc |= (1<<GPIOA_EN);
	    *rcc |= (1<<GPIOB_EN);
	    *rcc |= (1<<GPIOC_EN);

	    //pins a[11..4] as output
	    for(int i = 4; i < 12; i++){
	    	gpioa->moder &= ~(0b11<<(2*i));
	    	gpioa->moder |= 0b01<<(2*i);
	    }

	    //pins c[10..8] as output
		for(int i = 8; i < 11; i++){
			gpioc->moder &= ~(0b11<<(i*2));
			gpioc->moder |= (0b01<<(i*2));
		}
	    
}

/**
 * LcdInit()
 * Description:
 * 		Wait 40 MS
 * 		Set Function
 * 			8bit mode DL = 1
 * 			2Line mode N = 1
 * 			5x8 dots   F = 0
 * 		Set Function (again)
 * 		Display ON
 * 			DisplayOn D=1
 * 			CursorOn  C=1
 * 			BlinkOn   B=1
 * 		Display Clear (note longer delay)
 * 		Entry Mode Set
 * 			IncrementAddress I/D=1
 * 			ShiftOff S=0
 * Inputs: None
 * Outputs: None
 */
void LcdInit(){
	BusyMilli(40);
	//Set functions

	//do first step twice cause 8 bits of data
	gpioc->bsrr = RS_CMD|RW_WR|E_SET;
	
	//first instr
	uint8_t cmd = 0b00111000;
	gpioa->odr = (cmd<<4);
	BusyMilli(2);
	gpioc->bsrr = E_CLR;
	BusyMilli(2);
	//second instr
	gpioc->bsrr = E_SET;
	cmd = 0b00111000;
	gpioa->odr = (cmd<<4);
	BusyMilli(2);



	//display on
	//D - display on
	//C - cursor on
	//B - blink on
	LcdSetDisplay(true,false,false);

	//display clear
	LcdClear();

	//entry mode set
	//I/D - increment address off
	//S - shift off
	lcdWriteCmd(0b00000110);


}

/**
 * LcdClear()
 * Description:
 * 		Clear Display
 * Inputs: None
 * Outputs: None
 */
void LcdClear(){
	//clear display
	lcdWriteCmd(0x01);
	//wait for delay
	BusyMilli(40);

}

/**
 * LcdHome()
 * Description:
 * 		Move Cursor to the home position (0,0)
 * Inputs: None
 * Outputs: None
 */
void LcdHome(){
	//move cursor to home
	lcdWriteCmd(0b0000000010);
	//wait for delay
	BusyMilli(40);
}

/**
 * LcdSetDisplay()
 * Description:
 * 		Set/change the display options
 * 	Inputs:
 * 		onOff(bool) - true (display on), false (display off)
 * 		cursor(bool) - true (cursor on), false (cursor off)
 * 		blink(bool) - true (blink on), flase (blink off)
 * 	Outputs: none
 */
void LcdSetDisplay(bool onOff, bool cursor, bool blink){
	//set display options
	//D - display on
	//C - cursor on
	//B - blink on
	uint16_t cmd = 0x08;
	if(onOff){
	cmd |= 0x04;
	}
	if(cursor){
	cmd |= 0x02;
	}
	if(blink){
	cmd |= 0x01;
	}
	lcdWriteCmd(cmd);

}

/**
 * LcdSetPosition()
 * Description:
 * 		Set the position of the cursor.
 * 		Top left (0,0)     Top right (0,15)
 * 		Bottom left (1,0)  Bottom right (1,15)
 * 	Inputs:
 * 		row(uint8_t) - row index, 0-top, 1-bottom
 * 		col(uint8_t) - col index, 0-left, 1-right
 */
void LcdSetPosition(uint8_t row, uint8_t col){
	//set position of cursor
	//0x80 is the command to set the position
	//0x40 is the command to set the row
	//0x7F is the mask to clear the row
	uint8_t cmd = 0x80;
	if(row == 1){
		cmd |= 0x40;
	}
	cmd |= (col & 0x0F);
	lcdWriteCmd(cmd);
}

/**
 * LcdWriteChar()
 * Description:
 * 		Writes the character to screen according table in
 * 		documentation.
 * 		(See lcdWriteCmd for implementation details)
 * 	Input:
 * 		c(char) - char to write
 * 	Output: none
 */
void LcdWriteChar(char c){
	//set RS to data
	gpioc->bsrr = RS_DATA;
	//need to be in write mode
	gpioc->bsrr = RW_WR;
	//set E to 1
	gpioc->bsrr = E_SET;
	//write cmd to databus (read modify write)
	uint32_t reg = gpioa->odr;
	reg &= 0xFFFFF00F;
	reg |= (c<<4);
	gpioa->odr = reg;
	
	BusyMilli(2);
	//set E to 0
	gpioc->bsrr = E_CLR;
	//wait for delay
	BusyMilli(2);

}

/**
 * LcdWriteStr()
 * Description:
 * 		Writes string to the current console.
 * 		Note: no protection provided for overwriting end of screen
 * 	Input:
 * 		str(char*) - string to write
 * 	Output:
 * 		None
 */
void LcdWriteStr(const char * const str){
	//write string to console
	for(int i = 0; str[i] != '\0'; i++){
		LcdWriteChar(str[i]);
	}
}

/** ExtraCredit
 * LcdGetPosition()
 * Description:
 * 		Finds the current position of the cursor.
 * 	Inputs:
 * 		row(uint8_t*) - returned row location by reference
 * 		col(uint8_t*) - returned col location by reference
 */
void LcdGetPosition(uint8_t* const row, uint8_t* const col){
	//get position of cursor
	//0x80 is the command to set the position
	//0x40 is the command to set the row
	//0x7F is the mask to clear the row
	uint8_t cmd = 0x80;
	lcdWriteCmd(cmd);
	//read the position
	uint8_t pos = gpioa->idr;
	//mask the position
	pos &= 0x7F;
	//set the row and col
	*row = (pos & 0x40) >> 6;
	*col = pos & 0x0F;
}

//display on
void LcdDisplayOn(){
	//D - display on
	//C - cursor on
	//B - blink on
	LcdSetDisplay(true,false,false);
}

//display off
void LcdDisplayOff(){
	//D - display on
	//C - cursor on
	//B - blink on
	LcdSetDisplay(false,false,false);
}

/** EXTRA CREDIT
 * lcdWaitBusyFlag()
 * Description:
 * 		Blocks polling the BusyFlag until it reads '0'
 * 		Note: The mode of DB7 must be switched to input during polling
 * 				and back to output after polling is complete.
 */
static void lcdWaitBusyFlag(){

}
