#include "avr/io.h"
#include "avr/interrupt.h"

int main()
{
	PORTB |= 0X04;	//turn off PB2
	DDRB |= 0X04;	//set PB2 as output 

	TCCR0B = 0X05;		//set timer0 to CTC mode
	TCCR0A |= (1<<WGM01);		 //prescale = 1024
	OCR0A = 0XFF;	//compare value = 0XFF
	TIMSK0 = (1<<OCIE0A);	//enable timer0 compare interrupt
	sei();			//enable global interrupt
	
	while(1)
	{
	}
}

ISR (TIMER0_COMPA_vect)
{
	while(PINC & (1 << PINC3)) //wait for PINC3 input
		{
		}
	
	PORTB ^= (1<<2);	//toggle LED	
	int counter = 0;		//reset counter
	TCNT0 = 0;				//reset timer
	
	while(counter != 82)	//count 1.333 sec
	{
		while((TIFR0 &(1<<OCF0A))==0)
		{
		}
		TCNT0 = 0;
		TIFR0 |= (1<<OCF0A);	//reset interrupt flag register
		counter += 1;
	}
	
	PORTB ^= (1<<2);	//toggle LED
} 
