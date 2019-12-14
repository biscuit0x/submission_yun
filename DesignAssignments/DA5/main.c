/*
 * DA5.c
 * Author : jayne
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//#include <stdbool.h>
#include <stdio.h>
#include <string.h>
// Set up UART for printf();
#ifndef BAUD
#define BAUD 9600
#endif


#include "inc\STDIO_UART.c"
// Include nRF24L01+ library
#include "inc\nrf24l01.c"
#include "inc\nrf24l01-mnemonics.h"
#include "inc\spi.c"
void print_config(void);


void print_config(void)


volatile bool message_received = false;
volatile bool status = false;
void ADC_init (void);
volatile unsigned ADC_temp;
volatile char temp[20];


void ADC_init(void) {
ADMUX |= (1<<REFS0)| 
(1<<MUX2) ;
ADCSRA |= (1<<ADEN)|
 (0<<ADSC)| 
 (0<<ADATE)|// Auto Trigger off
 (0<<ADIF)| // Interrupt Flag reset
 (0<<ADIE)| // Interrupt Disabled
 (1<<ADPS2)| // ADC Prescaler Select 32
 (0<<ADPS1)| 
 (1<<ADPS0); 
}

void print_config(void)
{
	uint8_t data;
	printf("Startup successful\n\n nRF24L01+ configured as:\n");
	printf("-------------------------------------------\n");
	nrf24_read(CONFIG,&data,1);
	printf("CONFIG 0x%x\n",data);
	nrf24_read(EN_AA,&data,1);
	printf("EN_AA 0x%x\n",data);
	nrf24_read(EN_RXADDR,&data,1);
	printf("EN_RXADDR 0x%x\n",data);
	nrf24_read(SETUP_RETR,&data,1);
	printf("SETUP_RETR 0x%x\n",data);
	nrf24_read(RF_CH,&data,1);
	printf("RF_CH 0x%x\n",data);
	nrf24_read(RF_SETUP,&data,1);
	printf("RF_SETUP 0x%x\n",data);
	nrf24_read(STATUS,&data,1);
	printf("STATUS 0x%x\n",data);
	nrf24_read(FEATURE,&data,1);
	printf("FEATURE 0x%x\n",data);
	printf("-------------------------------------------\n\n");
}

void READ_ADC(void) {
	unsigned char i =4;
	ADC_TEMP = 0; //initialize
	while (i--) {
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		ADC_TEMP+= ADC;
		_delay_ms(100);
	}
	ADC_TEMP = ADC_TEMP/4; // Average
}

int main(void)
{
	char tx_message[32]; // Define string array
	strcpy(tx_message,"testing . . . ."); // Copy string into array
	// Initialize UART
	uart_init();
	// Initialize ADC
	ADC_init();
	// Initialize nRF24L01+ and print configuration info
	nrf24_init();
	print_config();
	// Start listening to incoming messages
	nrf24_start_listening();
	strcpy(tx_message,"Initializing Chat Room..."); // Copy string into array
	nrf24_send_message(tx_message);
	while (1)
	{
	if (message_received)
		{
				message_received = false;
				READ_ADC();
		// Message received, print it
		printf("Received : %s\n\n",nrf24_read_message());
	snprintf(temp,sizeof(temp),"%d\r\n",adc_temp); 
	// Send message as response
	_delay_ms(500);
	status = nrf24_send_message(temp);
	if (status == true) printf("Temperature Successfully Sent\n\n");
}
}
}
// Interrupt on IRQ pin
ISR(INT0_vect)
{
message_received = true;
}
