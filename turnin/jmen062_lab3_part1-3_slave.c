/*
 * CS122A_Lab3_Part1.c
 *
 * Created: 10/11/2019 3:13:43 PM
 * Author : Jason Mendoza
 */ 

#include <avr/io.h>
//#include "SPI_ATmega1284.h"
#include "timer.h"



void SPI_ServantInit(void){
	DDRB = 0x40; PORTB = 0xBF;
	SPCR |= (1 << SPE) | (1 << SPIE);
	SREG = 0x80;
	
}

unsigned char recievedData;
ISR(SPI_STC_vect){
	recievedData = SPDR;
}


 unsigned char output1;
 unsigned char output2;
 unsigned char output3 = 0x01;
 unsigned char output4 = 0xFF;
 unsigned char reverse = 0x00;
 unsigned char speed;
 unsigned char pattern;
 unsigned char truespeed;
 unsigned char gcd = 50;
 unsigned char counter = 0;
 

enum output {out1, out2, out3, out4} ostate;
void outputtick(){
	switch(ostate){
		case out1:
			if( pattern == 0x20 ){
				ostate = out2;
			}
			else if( pattern == 0x30 ){
				ostate = out3;
			}
			else if( pattern == 0x40){
				ostate = out4;
			}
			else{
				ostate = out1;
			}
			break;
		case out2:
			if( pattern == 0x20 ){
				ostate = out2;
			}
			else if( pattern == 0x30 ){
				ostate = out3;
			}
			else if( pattern == 0x40){
				ostate = out4;
			}
			else{
				ostate = out1;
			}
		break;
		case out3:
			if( pattern == 0x20 ){
				ostate = out2;
			}
			else if( pattern == 0x30 ){
				ostate = out3;
			}
			else if( pattern == 0x40){
				ostate = out4;
			}
			else{
				ostate = out1;
			}
			break;
		case out4:
			if( pattern == 0x20 ){
				ostate = out2;
			}
			else if( pattern == 0x30 ){
				ostate = out3;
			}
			else if( pattern == 0x40){
				ostate = out4;
			}
			else{
				ostate = out1;
			}
			break;
		
	}


	switch(ostate){
		case out1:
			PORTA = output1;
			break;
		case out2:
			PORTA = output2;
			break;
		case out3:
			PORTA = output3;
			break;
		case out4:
			PORTA = output4;
			break;
	}
}

enum patterns {one, two, three, four} state;	
void patterntick(){
	switch(state){
		case one:
			if( pattern == 0x20 ){
				state = two;
			}
			else if( pattern == 0x30 ){
				state = three;
			}
			else if( pattern == 0x40){
				state = four;
			}
			else{
				state = one;
			}
			break;
		case two:
			if( pattern == 0x20 ){
				state = two;
			}
			else if( pattern == 0x30 ){
				state = three;
			}
			else if( pattern == 0x40){
				state = four;
			}
			else{
				state = one;
			}
			break;
		case three:
			if( pattern == 0x20 ){
				state = two;
			}
			else if( pattern == 0x30 ){
				state = three;
			}
			else if( pattern == 0x40){
				state = four;
			}
			else{
				state = one;
			}
			break;
		case four:
			if( pattern == 0x20 ){
				state = two;
			}
			else if( pattern == 0x30 ){
				state = three;
			}
			else if( pattern == 0x40){
				state = four;
			}
			else{
				state = one;
			}
			break;	
		
	}
	switch(state){
		case one:
		if(counter >=  truespeed){
			if(output1 == 0xF0){
				output1 = 0x0F;
			}
			else{
				output1 = 0xF0;
			}
			counter = 0;
		}
		else {
			counter++;
		}
			break;
		case two:
		if(counter >= truespeed){
			if(output2 == 0xAA){
				output2 = 0x55;
			}
			else{
				output2 = 0xAA;
			}
			counter = 0;
		}
		else{
			counter++;	
		}
			break;
		case three:
		if(counter >= truespeed){
			if(output3 >= 0x80){
				reverse = 0x00;
			}
			else if(output3 <= 0x01 || output3 == 0x00 ){
				reverse = 0x01;
			}
			if(reverse == 0x00){
				output3 = output3 / 2;
			}
			else if(reverse == 0x01){
				output3 = output3 * 2 ;
			}
			counter = 0;
		}
		else{
			counter++;	
		}
			break;
		case four:
		if(counter >= truespeed){
			if(output4 == 0xFF){
				output4 = 0x00;
			}
			else{
				output4 = 0xFF;
			}
			counter = 0;
		}
		else{
			counter++;
		}
			break;	
	}
}


int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0x00; PORTB = 0xFF;
	
    SPI_ServantInit();
	
	TimerSet(50);
	TimerOn();

	
	
    while (1) 
    {
		
	speed = recievedData;
	pattern = recievedData;
	
	speed = speed & 0x0F;
	pattern = pattern & 0xF0;
	
	if(speed == 0x01){
		truespeed = 0;
	}
	else if(speed == 0x02){
		truespeed = 2;
	}
	else if(speed == 0x03){
		truespeed = 5;
	}
	else if(speed == 0x04){
		truespeed = 10;
	}
	else if(speed == 0x05){
		truespeed = 20;
	}
	else if(speed == 0x06){
		truespeed = 40;
	}
	
	patterntick();
	outputtick();
	while(!TimerFlag){};
	TimerFlag = 0;
    }
}


