
/*
 * DA2A_C.c
 *
 * Created: 11/3/2019 2:46:21 PM
 * Author : jayne
 */ 

#include <avr/io.h>


int main(void)
{ 
	DDRB = 1<<3;
	TCCR1A = 0;
	TCCR1B = 4;	
	while (1) 
    {	
		PORTB = 8;
		TCNT1 = 49911;	//40DC
		while(TCNT1 != 0); //wait
		PORTB = 0;
		TCNT1 = 42108; //60DC
		while(TCNT1 != 0); //wait
    }
}


