/*
 * GccApplication1.c
 *
 * Created: 12/12/2019 7:49:40 PM
 * 1. Write a C AVR program that will monitor the LM34/35 connected to an Analog pin (PC5) 
 to display the temperature in F on the serial terminal every 1 sec. 
 Use a timer with interrupt for the 1 sec delay.
  Use a FTDI chip for serial to USB conversion.
 */ 


#define F_CPU 16000000UL
#define UBRR_9600 103// for 16Mhz with .2% error

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

void read_adc(void);// Function Declarations
void adc_init(void);
void USART_init( unsigned intubrr);
void USART_tx_string( char *data ); 
void TIMER_init(void);
volatile unsigned int adc_temp;
char outs[20];

int main(void) {
	adc_init();// Initialize the ADC (Analog / Digital Converter)
	USART_init(UBRR_9600); // Initialize the USART (RS232 interface)
	TIMER_init();
	while(1)
	{
	}
}

/* INIT USART (RS-232)  */
void USART_init( unsigned intubrr) 
{
	UBRR0H = (unsigned char)(intubrr>>8);
	UBRR0L = (unsigned char)intubrr;
	UCSR0B = (1 << TXEN0); // Enable receiver, transmitter & RX interrupt
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);//asynchronous 8 N 1
}

ISR (TIMER1_OVF_vect){
	adc_temp= 0;
	TCNT1 = (0xFFFF - ((F_CPU/1024)*1) - 60); //set counter for 1 sec
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		ADCSRA |= (1<<ADIF); //clear ADC interrupt flag
		adc_temp = ADCL;
		adc_temp = adc_temp | (ADCH<<8); 
		snprintf(outs, sizeof(outs),"%3d\r\n", adc_temp);// print it
		USART_tx_string(outs);

}
void TIMER_init(void) {
	TCNT1 = (0xFFFF - ((F_CPU/1024)*1) - 60); //set counter for 1 sec
	TIMSK1 |= (1 << TOIE0); // Enable Timer1 Interrupt for Timer1
	sei(); // Enable Global Interrupt
	TCCR1B |= (1<<CS12)|(1<<CS10); // set prescaler = 1024 & start timer
}

void adc_init(void){/** Setup and enable ADC **/
	ADMUX = (0<<REFS1)|   // Reference Selection Bits
	(1<<REFS0)|  // AVcc-external cap at AREF
	(0<<ADLAR)| // ADC Left Adjust Result
	(1<<MUX2)|  // ANalogChannel Selection Bits
	(0<<MUX1)|  // ADC2 
	(1<<MUX0);
	ADCSRA = (1<<ADEN)|// ADC ENable
	(0<<ADSC)| // ADC Start Conversion
	(0<<ADATE)| // ADC Auto Trigger Enable
	(0<<ADIF)|// ADC Interrupt Flag
	(0<<ADIE)| // ADC Interrupt Enable
	(1<<ADPS2)| // ADC PrescalerSelect Bits
	(0<<ADPS1)|(1<<ADPS0);   // Select Channel
	}
	
void USART_sendChar(char ch) {
	while (!(UCSR0A & (1<<UDRE0))); // while data reg is not empty: hold
	UDR0 = ch; // place character into reg
}


void USART_sendString(char* str) {
	while ((*str != '\0')) { // while not the end of the string
		while (!(UCSR0A & (1<<UDRE0))); // while data reg is not empty: hold
		USART_sendChar(*str); //take in character to reg
		str++; // next character
	}
}

/* SEND A STRING TO THE RS-232 */
void USART_tx_string( char *data ) 
{ 
	while ((*data != '\0')) 
	{
		while (!(UCSR0A & (1 <<UDRE0)));
		UDR0 = *data;data++;
	}   
}