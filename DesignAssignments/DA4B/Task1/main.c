/*
 * Task1.c
 * Author : jayne
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile int val = 0; //adc value

void time_i(void) { //CTC mode, prescaler = 8
	TCCR1B = (1<<WGM12) | (1<<CS11); 
}

void adc_i(void) {
	ADCSRA |= (1<<ADEN)| (1<<ADPS2)| (1<<ADPS1)| (1<<ADPS0);//prescale = 128 
	sei(); //global interrupt
	ADMUX |= (1<<REFS0); //reference voltage
	PORTC |= (1<<PORTC1); //pull up
	DDRC &= (0<<PORTC0); // PC0 as input
}

int main(void)
{
	adc_i(); // initialize ADC and timer
	PORTB = 0;
	DDRB = 15; //PB0-3 as output
	time_i(); 
	//loop
	while (1)
	{
		ADCSRA |= (1<<ADSC); //conversion
		while((ADCSRA&(1<<ADIF))==0); 
		ADCSRA |= (1<<ADIF); //reset
		val = ADC; // store ADC to val for debugging
		OCR1A = 10*val; //95% PWM
		PORTB = 9;
		while(!(TIFR1 & (1<<OCF1A))); //motor rotation
			TIFR1 |= (1 << OCF1A); 
			PORTB = 3;
		while(!(TIFR1 & (1<<OCF1A)));
			TIFR1 |= (1 << OCF1A);
			PORTB = 6;
		while(!(TIFR1 & (1<<OCF1A)));
			TIFR1 |= (1 << OCF1A);
			PORTB = 12;
		while(!(TIFR1 & (1<<OCF1A)));
			TIFR1 |= (1 << OCF1A);
	}		
}
