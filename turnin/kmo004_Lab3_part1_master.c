/*	Author: kmo004
 *  Partner(s) Name: Jason Mendoza
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "bit.h"
#include "lcd.h"
#include "keypad.h"
#include "timer.h"
#endif

void SPI_MasterInit(void) {
	DDRB = 0xBF; PORTB = 0x40;
	
	SPCR |= (1<<SPE) | (1<<MSTR) | (1<<SPR0);
	
	SREG = 0x80;
}

void SPI_MasterTransmit(char cData){
	
	PORTB = SetBit(PORTB,4,0);
	
	SPDR = cData;
	
	while(!(SPSR & (1<<SPIF))){;
	}
	PORTB = SetBit(PORTB,4,1);
}

unsigned light = 0x00;

enum states{l1,l2,l3,l4,l5,l6,l7,l8}state;
void Tick(){
	switch(state){
		case l1:
			if(state == l1){
				state = l2;
			}
			else{
				state = l1;
			}
			break;
		case l2:
			if(state == l2){
				state = l3;
			}
			else{
				state = l2;
			}
			break;
		case l3:
			if(state == l3){
				state = l4;
			}
			else{
				state = l3;
			}
			break;
		case l4:
			if(state == l4){
				state = l5;
			}
			else{
				state = l4;
			}
			break;
		case l5:
			if(state == l5){
				state = l6;
			}
			else{
				state = l5;
			}
			break;
		case l6:
			if(state == l6){
				state = l7;
			}
			else{
				state = l6;
			}
			break;
		case l7:
			if(state == l7){
				state = l8;
			}
			else{
				state = l7;
			}
			break;
		case l8:
			if(state == l8){
				state = l1;
			}
			else{
				state = l8;
			}
			break;
		default:
			state = l1;
			break;
		
	}
	switch(state){
		case l1:
			light = 0x01;
			break;
		case l2:
			light = 0x02;
			break;
		case l3:
			light = 0x04;
			break;
		case l4:
			light = 0x08;
			break;
		case l5:
			light = 0x10;
			break;
		case l6:
			light = 0x20;
			break;
		case l7:
			light = 0x40;
			break;
		case l8:
			light = 0x80;
			break;
			
		default:
			light = 0xFF;
			break;
	}
}


int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0x00; PORTB = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    
    SPI_MasterInit();
    
    state = l1;
    TimerSet(500);
	TimerOn();

    /* Insert your solution below */
    while (1) {
		
		Tick();
		//light = 0xFF;
		SPI_MasterTransmit(light);
		
		while (!TimerFlag){}
		TimerFlag = 0;

    }
    return 1;
}
