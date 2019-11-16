#include "avr/io.h"

int main()
{
    DDRB = 0X04;	//PB2 as output
	PORTB = 0X04;
	TCCR0A = 0;		//set timer0 normal mode
	TCCR0B = 0X05;	//prescaler = 1024
	int counter = 0; //initialize counter
	
	while(1)
	{
		while(PINC & (1 << PINC3)) //wait for PINC3 input
		{
		}
		PORTB ^= (1<<2);	//toggle LED
		counter = 0;	//reset counter
		TCNT0 = 0;		//reset timer
		while(counter != 82)	//count 1.333 sec
		{
			while(TCNT0 != 255)
			{
			}
			counter++;
			TCNT0 = 0;
		}
		PORTB ^= (1<<2);	//toggle LED
	}
}
