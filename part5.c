/*	Author: Jarvis Bui
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>

#include "io.c"
#include "io.h"
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

//char* string = "*          *      *     *     *      ";
//unsigned char count = 0;
//unsigned char positions[32];
//unsigned char screen[32];
//unsigned char position = 0;
//unsigned char running = 1;
//unsigned int i;

unsigned char temp;
unsigned char top;
unsigned char bottom;
unsigned char user;
unsigned char oTop;
unsigned char oBottom;
int pause;
int row;
enum cursorStates{cStart, up, down}cState;
enum objectStates{oStart, move}oState;
enum gameStates{gStart, play, wait, lost, reset}gState;
void moveCursor(){
	switch(cState){
		case cStart:
			cState = up;
			break;
		case up:
			if(temp == 0x02){
				cState = down;
			}

			break;
		case down:
			if(temp == 0x01){
				cState = up;
			}
			break;
	}

	switch(cState){
		case cStart:
			break;
		case up:
			row = 1;
			LCD_Cursor(1);
			user = 1;
			break;
		case down:
			row = 2;
			LCD_Cursor(17);
			user = 17;
			break;
	}
}

void makeObstacles(){
	switch(oState){
		case oStart:
			oState = move;
			break;
		case move:
			if(top != 1 && bottom != 17){
				top = top - 1;
				bottom = bottom - 1;
			}
			else if(top == 1){
				top = 16;
			}
			else if(top == 17){
				bottom = 32;
			}
			break;
	}
	switch(oState){
		case oStart:
			break;
		case move:
			LCD_ClearScreen();
			LCD_Cursor(top);
			LCD_WriteData('#');
			oTop = top;
			LCD_Cursor(bottom);
			LCD_WriteData('#');
			oBottom = bottom;
			break;
	}
}

void game(){
	switch(gState){
		case gStart:
			gState = play;
			break;
		case play:
			if(user == oTop || user == oBottom){
				gState =lost;
			}
			else{
				gState = play;
			}
			break;
		case lost:
			gState = wait;
			break;
		case wait:
			if(temp == 0x04){
				gState = reset;
			}
			break;
		case reset:
			gState = play;
			break;
	}
	
	switch(gState){
		case gStart:
			break;
		case play:
			moveCursor();
			break;
		case lost:
			LCD_DisplayString(1,(unsigned char*)"u are a loser");
			pause = 1;
			break;
		case wait:
			break;
		case reset:
			LCD_ClearScreen();
			pause = 0;
			break;
	}

}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
	temp = 0x00;
	top = 15;
	bottom = 26;
	pause = 0;
	
	unsigned long cursorTime = 50;
	unsigned long obstacleTime = 500;
	unsigned long timerPeriod = 50;

	cState = cStart;
	oState = oStart;
	gState = gStart;

	TimerSet(timerPeriod);
	TimerOn();
	LCD_init();

    while (1) {
	 	temp = ~PINB & 0x07;	
		if(pause == 0){
			if(cursorTime >= 50){
				game();
				cursorTime = 0;
			}
			if(obstacleTime >= 500){
				makeObstacles();
				obstacleTime = 0;
			}
		}
		while(!TimerFlag){}
		TimerFlag = 0;
		cursorTime += timerPeriod;
		obstacleTime += timerPeriod;
	}
/*
	switch(key){
		case '\0': PORTB = 0x1F; break; //all 5 leds on 
		case '1': PORTB = 0x01; break; //hex values from here on out 
		case '2': PORTB = 0x02; break;
		case '3': PORTB = 0x03; break;
		case '4': PORTB = 0x04; break;
		case '5': PORTB = 0x05; break;
		case '6': PORTB = 0x06; break;
		case '7': PORTB = 0x07; break;
		case '8': PORTB = 0x08; break;
		case '9': PORTB = 0x09; break;
		case 'A': PORTB = 0x0A; break;
		case 'B': PORTB = 0x0B; break;
		case 'C': PORTB = 0x0C; break;
		case 'D': PORTB = 0x0D; break;
		case '*': PORTB = 0x0E; break;
		case '0': PORTB = 0x00; break;
		case '#': PORTB = 0x0F; break;
		default: PORTB = 0x1B; break; // should never ovvur
	
		while(!TimerFlag);
		TimerFlag = 0;
	}
	*/
    
    return 1;
}
