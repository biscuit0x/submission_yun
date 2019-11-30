/*
 * DA3C.c
 * Kyungseo Yun
 */ 

#define F_CPU 16000000UL
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL)))-1)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

char* string; //string pointer 

void USART_init ( void )
{
	UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8);	/*set BAUD rate*/
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8bit data */
	UCSR0B = _BV(RXEN0) | _BV(TXEN0); /* Enable RX and TX */
}

void USART_send (char ch)
{
	//wait until UDR0 is empty
	while (! (UCSR0A & (1<<UDRE0)));
	UDR0 = ch ; //transmit ch
}

void USART_putstring (char* StringPtr )
{
	while (*StringPtr != 0x00){
	USART_send(*StringPtr);	//send one character at a time
	StringPtr++;
	}
}

ISR (TIMER1_OVF_vect)
{
	string = "random string \n"; //print random string
	USART_putstring(string);
	
	string = "1234 \n";	//print random integer
	USART_putstring(string);
	
	string = "5.6789 \n\n";//print random float
	USART_putstring(string);
	TCNT1 = 49911;	//reset timer
}

int main(void)
{
	USART_init(); //UART initialization
	
	TIMSK1 = (1<<TOIE1); //enable overflow interrupt
	TCNT1 = 49911;	//set counter value for 1 sec
	TCCR1B = 0X05;	//begin counter with prescalar = 1024
	sei(); //enable interrupts
    while (1) 
    {
    }
}

