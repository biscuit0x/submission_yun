#include "avr/io.h"
#include "avr/interrupt.h"

int main()
{
    DDRB |= 0X04;	//set PB2 as output
	PORTB |= 0X04;
	TCCR0A = 0;		//timer0 normal mode
	TCCR0B = 0X05; //prescaler = 1024
	TIMSK0 = (1<<TOIE0); //enable overflow interrupt
	sei();	//enable global interrupt
	while(1)	//wait for interrupt
	{

	}
}

ISR (TIMER0_OVF_vect)
{
	while(PINC & (1 << PINC3)) //wait for PINC3 input
		{
		}
	
	PORTB ^= (1<<2);	//toggle LED
	int counter = 0;	//reset counter&timer
	TCNT0 = 0;	
	
	while(counter != 82) //count 1.333 sec
	{
		while(TCNT0 != 255)
		{
		}
		TCNT0 = 0; 
		counter += 1;
	}
	PORTB ^= (1<<2);	//reset timer
} 

