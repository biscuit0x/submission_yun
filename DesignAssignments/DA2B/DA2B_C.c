#include <avr/io.h>
#include <avr/interrupt.h>

int main()
{
	PORTB = 1<<2;	//set PB2 (led off)
	DDRB = 1<<2;	//PB2 as an output
	PORTD = 1<<2;	//PD2 pull-up activated
	EICRA = 0X2;		//make INT0 falling edge triggered
	
	EIMSK = (1<<INT0);	//enable external interrupts
	sei ();				//enable interrupts
	
	while (1);	//wait here
}

ISR (INT0_vect) //ISR for external interrupt 0 
{
	TCCR1A = 0;
	TCCR1B = 5; //prescaler = 1024
	TCNT1 = 0;	//begin timer
	PORTB ^= (1<<2); //turn on LED
	while(TCNT1 < 20928); //count 1.333 sec
	PORTB ^= (1<<2); //turn off LED
}