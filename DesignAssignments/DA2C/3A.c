#include "avr/io.h"
#include "avr/interrupt.h"

int main()
{
	PORTB |= 0X08;	//toggle PB3
	DDRB |= 0X08;	//set PB3 as output 

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
	int counter = 0;		//reset counter
	TCNT0 = 0;				//reset timer

	while(counter != 15)	//DC40, 0.25 sec
	{
		while((TIFR0 &(1<<OCF0A))==0)
		{
		}
		TCNT0 = 0;
		TIFR0 |= (1<<OCF0A);	//reset interrupt flag register
		counter += 1;
	}
	
	PORTB ^= (1<<3);	//toggle PB3
	counter = 0;		//reset counter
	TCNT0 = 0;			//reset timer
	//TIFR0 |= (1<<OCF0A);
	while(counter != 23) //DC60, 0.375 sec
	{
		while((TIFR0&(1<<OCF0A))==0)
		{
		}
		TCNT0 = 0;
		TIFR0 |= (1<<OCF0A);	//reset interrupt flag register
		counter += 1;	
	}

} 


