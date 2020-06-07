/*	Author: lab
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
#include "bit.h"
#include "io.c"
#include "io.h"
#include "timer.h"
#include "scheduler.h"
#include "keypad.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
unsigned char save;
unsigned char key;
unsigned char x;
unsigned char temp;
unsigned char flag;
enum states{output, press}state;
int tick(int state){
	unsigned char input;
	input = GetKeypadKey();
	temp = 0x00;
	switch(state){
		case output:
			switch(input){
				if(flag == 0){
					LCD_cursor(1);
					LCD_WriteData(temp + '0');
				}
				case '1':
					flag = 1;
					temp = 0x01;
					LCD_Cursor(1);
					LCD_WriteData(temp + '0');
					break;

				case '2':
					flag = 1;
					temp = 0x02;
					LCD_Cursor(1);
					LCD_WriteData(temp + '0');
					break;

				case '3':
					flag = 1;
					temp = 0x03;
					LCD_Cursor(1);
					LCD_WriteData(temp + '0');
					break;

				case '4':
					flag = 1;
					temp = 0x04;
					LCD_Cursor(1);
					LCD_WriteData(temp + '0');
					break;

				case '5':
					flag = 1;
					temp = 0x05;
					LCD_Cursor(1);
					LCD_WriteData(temp + '0');
					break;

				case '6':
					flag = 1;
					temp = 0x06;
					LCD_Cursor(1);
					LCD_WriteData(temp + '0');
					break;

				case '7':
					flag = 1;
					temp = 0x07;
					LCD_Cursor(1);
					LCD_WriteData(temp + '0');
					break;

				case '8':
					flag = 1;
					temp = 0x08;
					LCD_Cursor(1);
					LCD_WriteData(temp + '0');
					break;

				case '9':
					flag = 1;
					temp = 0x09;
					LCD_Cursor(1);
					LCD_WriteData(temp + '0');
					break;

				case 'A':
					flag =2;
					temp = 0x0A;
					LCD_Cursor(1);
					LCD_WriteData(temp + 0x37);
					break;

				case 'B':
					flag =2;
					temp = 0x0B;
					LCD_Cursor(1);
					LCD_WriteData(temp + 0x37);
					break;

				case 'C':
					flag = 2;
					temp = 0x0C;
					LCD_Cursor(1);
					LCD_WriteData(temp + 0x37);
					break;

				case 'D':
					flag =2;
					temp = 0x0D;
					LCD_Cursor(1);
					LCD_WriteData(temp + 0x37);
					break;

				case '*':
					flag = 3;
					temp = 0x0E;
					LCD_Cursor(1);
					LCD_WriteData(temp + 0x1C);
					break;

				case '0':
					flag = 1;
					temp = 0x00;
					LCD_Cursor(1);
					LCD_WriteData(temp + '0');
					break;

				case '#':
					flag = 4;
					temp = 0x0F;
					LCD_Cursor(1);
					LCD_WriteData(temp + 0x14);
			}
			state = press;
			PORTB = temp;
			LCD_Cursor(1);
			LCD_WriteData(temp + '0');
			save = temp;
			break;

		case press:
			LCD_Cursor(1);
			switch(flag){
				case 1:
					LCD_WriteData(save + '0');
					break;
				case 2:
					LCD_WriteData(save + 0x37);
					break;
				case 3:
					LCD_WriteData(save + 0x1C);
					break;
				case 4:
					LCD_WriteData(save + 0x14);
					break;
			}
			if(input != NULL){
				state = output;
			}
			else{
				state = press;
			}
			break;
	}
	return state;

}

int main(void) {
    /* Insert DDR and PORT initializations */

	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0xFF; PORTA = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
	DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
	unsigned long int SMTick1_calc = 500;
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(tmpGCD, SMTick1_calc);
	unsigned long int GCD = tmpGCD;
	unsigned long int SMTick1_period = SMTick1_calc/GCD;
	static task task1;
	task *tasks[] = {&task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	task1.state = 0;
	task1.period = SMTick1_period;
	task1.elapsedTime = SMTick1_period;
	task1.TickFct = &tick;

	TimerSet(GCD);
	TimerOn();
	LCD_init();
    while (1) {
	    int i;
	for( i = 0; i < numTasks; i++){
		if(tasks[i]->elapsedTime == tasks[i]->period){
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
		}
		tasks[i]->elapsedTime += 1;
	}

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
    }
    return 1;
}
