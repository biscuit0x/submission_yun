#include "avr/io.h"
#include "avr/interrupt.h"

int main()
{
    DDRB |= 0X08;	//set PB3 as output 
	PORTB |= 0X08;	//turn off PB3
	TCCR0A = 0;
	TCCR0B = 0X05; //prescale = 1024
	TIMSK0 = (1<<TOIE0);//enable TIMER0 overflow interrupt
	sei();			//enable global interrupt
	while(1)
	{

	}
}

ISR (TIMER0_OVF_vect)
{
	int counter = 0;		//reset counter
	TCNT0 = 0;				//reset timer
	while(counter != 15)	//DC40, 0.25 sec
	{
		while(TCNT0 != 255)
		{
		}
		TCNT0 = 0;
		counter += 1;
	}
	
	PORTB ^= (1<<3);	//toggle PB3
	counter = 0;		//reset counter
	TCNT0 = 0;			//reset timer
	
	while(counter != 23) //DC60, 0.375 sec
	{
		while(TCNT0 != 255)
		{
		}
		TCNT0 = 0; 
		counter += 1;
	}
} 

