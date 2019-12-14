/*
 * USART_RS232_C_file.c
 * http://www.electronicwings.com
 *
 */ 

#include "uart.h"						/* Include USART header file */

void USART_Init( void )
{
UBRR0H = 0; // not needed
UBRR0L = 8; // used for 115200
UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
UCSR0B = _BV(RXEN0) | _BV(TXEN0); /* Enable RX and TX */
}

char USART_RxChar()									/* Data receiving function */
{
	while (!(UCSR0A & (1 << RXC0)));					/* Wait until new data receive */
	return(UDR0);									/* Get and return received data */ 
}

void USART_TxChar(char data)						/* Data transmitting function */
{
	UDR0 = data;										/* Write data to be transmitting in UDR */
	while (!(UCSR0A & (1<<UDRE0)));					/* Wait until data transmit and buffer get empty */
}

void USART_SendString(char *str)					/* Send string of USART data function */ 
{
	int i=0;																	
	while (str[i]!=0)
	{
		USART_TxChar(str[i]);						/* Send each char of string till the NULL */
		i++;
	}
}

void uart_putc(unsigned char c){
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = c; 
}
void uart_puts(char *s){
while(*s){
uart_putc(*s);
s++;
}
}