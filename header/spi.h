#ifndef SPI_H
#define SPI_H

void SPI_MasterInit(void) {
	DDRB = 0xBF; PORTB = 0x40;
	
	SPCR |= (1<<SPE) | (1<<MSTR) | (1<<SPR0);
	
	SREG = 0x80;
}

void SPI_MasterTransmit(char cData){
	
	PORTB = SetBit(PORTB,4,0);
	
	SPDR = cData;
	
	while(!(SPSR & (1<<SPIF)));
	
	PORTB = SetBit(PORTB,4,1);
}


void SPI_SlaveInit(void){
	
	DDRB = 0x40; PORTB = 0xBF;
	
	SPCR |= (1<<SPE) | (1<<SPIE);
	
	SREG = 0x80;
}

unsigned char receivedData = 0x00;
ISR(SPI_STC_vect){
	receivedData = SPDR;
}
