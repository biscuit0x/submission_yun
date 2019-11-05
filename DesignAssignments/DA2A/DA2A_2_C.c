#include <avr/io.h>


int main(void)
{
   DDRB |= (1<<2);
   PORTB |= (1<<2);
   TCCR1A = 0;
   TCCR1B = 5; //prescaler = 1024
    while (1) 
    {
		if(!(PINC & (1 << PINC3))){	//wait for PC3 input
		PORTB = 0;	//turn on LED
		TCNT1 = -20928;
		while(TCNT1 != 0);  //count 1.333 sec
		}
		else//wait
		PORTB = 0x04; //turn off LED
	}
}

