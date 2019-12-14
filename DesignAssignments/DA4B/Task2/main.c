/*
 * Task2.c

 * Author : jayne
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

volatile int val = 0; //adc value

void time_i(void) { //timer1 setup
	TCCR1A = (1<<COM1A1) | (1<<COM1B1) |(1<<WGM11) ; //fast timer
TCCR1B = (1<<WGM13) |(1<<WGM12) |(1<<CS11) |(1<<CS10); //PWM set up w/ prescale = 64
}

void adc_i(void) {
	ADCSRA |= (1<<ADEN)| (1<<ADPS2)| (1<<ADPS1)| (1<<ADPS0);//prescale = 128 
	ADMUX |= (1<<REFS0); //reference voltage
}


int main(void)
{
	adc_i(); // initialize adc and timer
	time_i();
	while (1) //loop
	{
		ADCSRA |= (1<<ADSC); //conversion
		while((ADCSRA&(1<<ADIF))==0); 
		ADCSRA |= (1<<ADIF); //reset
		ICR1 = 4999; //50Hz
		DDRB |= (1<<PB1); //PB1 as output
		OCR1A = ADC; 
		_delay_ms(100); 
	}

}
